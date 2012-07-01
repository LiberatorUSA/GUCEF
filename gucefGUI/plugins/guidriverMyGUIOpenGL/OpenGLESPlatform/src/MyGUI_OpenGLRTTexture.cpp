/*!
	@file
	@author		Albert Semenov
	@date		12/2009
*/

#include "MyGUI_OpenGLRTTexture.h"
#include "MyGUI_OpenGLRenderManager.h"
#include "MyGUI_OpenGLDiagnostic.h"

#include <GLES/gl.h>

#define GL_GLEXT_PROTOTYPES
#include <GLES/glext.h>

namespace MyGUI
{

	OpenGLRTTexture::OpenGLRTTexture(unsigned int _texture, int width, int height) :
		mTextureID(_texture),
		mWidth(width),
		mHeight(height),
		mFBOID(0),
		mRBOID(0)
	{
		int miplevel = 0;
		glBindTexture(GL_TEXTURE_2D, mTextureID);
		//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_WIDTH, &mWidth);    // Unsupported by OpenGL ES, using param
		//glGetTexLevelParameteriv(GL_TEXTURE_2D, miplevel, GL_TEXTURE_HEIGHT, &mHeight);  // Unsupported by OpenGL ES, using param
		glBindTexture(GL_TEXTURE_2D, 0);

		mRenderTargetInfo.maximumDepth = 1.0f;
		mRenderTargetInfo.hOffset = 0;
		mRenderTargetInfo.vOffset = 0;
		mRenderTargetInfo.aspectCoef = float(mHeight) / float(mWidth);
		mRenderTargetInfo.pixScaleX = 1.0f / float(mWidth);
		mRenderTargetInfo.pixScaleY = 1.0f / float(mHeight);

		// create a framebuffer object, you need to delete them when program exits.
		glGenFramebuffersOES(1, &mFBOID);
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFBOID);

		// create a renderbuffer object to store depth info
		// NOTE: A depth renderable image should be attached the FBO for depth test.
		// If we don't attach a depth renderable image to the FBO, then
		// the rendering output will be corrupted because of missing depth test.
		// If you also need stencil test for your rendering, then you must
		// attach additional image to the stencil attachement point, too.
		glGenRenderbuffersOES(1, &mRBOID);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, mRBOID);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, mWidth, mHeight);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, 0);

		// attach a texture to FBO color attachement point
		glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, mTextureID, 0);

		// attach a renderbuffer to depth attachment point
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, mRBOID);

		glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0);
	}

	OpenGLRTTexture::~OpenGLRTTexture()
	{
		if (mFBOID != 0)
		{
			glDeleteFramebuffersOES(1, &mFBOID);
			mFBOID = 0;
		}
		if (mRBOID != 0)
		{
			glDeleteRenderbuffersOES(1, &mRBOID);
			mRBOID = 0;
		}
	}

	void OpenGLRTTexture::begin()
	{
		//glPushAttrib(GL_VIEWPORT_BIT); // not supported by OpenGL ES

		glBindFramebufferOES(GL_FRAMEBUFFER_OES, mFBOID);

		glViewport(0, 0, mWidth, mHeight);

		OpenGLRenderManager::getInstance().begin();
		glOrthof(-1, 1, 1, -1, -1, 1);
		glClear(GL_COLOR_BUFFER_BIT/* | GL_DEPTH_BUFFER_BIT*/);
	}

	void OpenGLRTTexture::end()
	{
		OpenGLRenderManager::getInstance().end();

		glBindFramebufferOES(GL_FRAMEBUFFER_OES, 0); // unbind

		//glPopAttrib();
	}

	void OpenGLRTTexture::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		OpenGLRenderManager::getInstance().doRender(_buffer, _texture, _count);
	}

} // namespace MyGUI
