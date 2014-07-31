/*!
	@file
	@author		George Evmenov
	@date		07/2009
	@module
*/

#include "MyGUI_OpenGLVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_OpenGLDiagnostic.h"

#include <GLES/gl.h>

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	OpenGLVertexBuffer::OpenGLVertexBuffer() :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK),
		mBufferID(0),
		mSizeInBytes(0),
        m_pData(NULL)
	{
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		destroy();
	}

	void OpenGLVertexBuffer::setVertexCount(size_t _count)
	{
		if (_count != mNeedVertexCount)
		{
			mNeedVertexCount = _count;
			destroy();
			create();
		}
	}

	size_t OpenGLVertexBuffer::getVertexCount()
	{
		return mNeedVertexCount;
	}

	Vertex* OpenGLVertexBuffer::lock()
	{
		return (Vertex*)m_pData;
	}

	void OpenGLVertexBuffer::unlock()
	{
		MYGUI_PLATFORM_ASSERT(mBufferID, "Vertex buffer in not created");

		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);

        glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, m_pData, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//GLboolean result = glUnmapBufferARB(GL_ARRAY_BUFFER_ARB);

		//MYGUI_PLATFORM_ASSERT(result, "Error unlock vertex buffer");
	}

	void OpenGLVertexBuffer::destroy()
	{
		if (mBufferID != 0)
		{
            if ( m_pData )
            {
                delete[] m_pData;
                m_pData = NULL;
            }
		    glDeleteBuffers(1, &mBufferID);
			mBufferID = 0;
		}
	}

	void OpenGLVertexBuffer::create()
	{
		MYGUI_PLATFORM_ASSERT(!mBufferID, "Vertex buffer already exist");

		mSizeInBytes = mNeedVertexCount * sizeof(MyGUI::Vertex);
		//void* data = 0;
        m_pData = new unsigned char[mSizeInBytes];

		glGenBuffers(1, &mBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, mBufferID);
		glBufferData(GL_ARRAY_BUFFER, mSizeInBytes, m_pData, GL_DYNAMIC_DRAW);

		// check data size in VBO is same as input array, if not return 0 and delete VBO
		int bufferSize = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
		if (mSizeInBytes != bufferSize)
		{
			destroy();
			MYGUI_PLATFORM_EXCEPT("Data size is mismatch with input array");
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} // namespace MyGUI
