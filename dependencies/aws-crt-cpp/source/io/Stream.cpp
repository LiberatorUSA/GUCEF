/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/crt/StlAllocator.h>
#include <aws/crt/io/Stream.h>

#include <aws/io/stream.h>

namespace Aws
{
    namespace Crt
    {
        namespace Io
        {
            InputStream::~InputStream()
            {
                // DO NOTHING: for now. But keep this here because it has to be virtual, and we may have
                // resources to clean up in the future.
            }

            int InputStream::s_Seek(aws_input_stream *stream, int64_t offset, enum aws_stream_seek_basis basis)
            {
                auto impl = static_cast<InputStream *>(stream->impl);

                if (impl->SeekImpl(offset, static_cast<StreamSeekBasis>(basis)))
                {
                    return AWS_OP_SUCCESS;
                }

                return AWS_OP_ERR;
            }

            int InputStream::s_Read(aws_input_stream *stream, aws_byte_buf *dest)
            {
                auto impl = static_cast<InputStream *>(stream->impl);

                if (impl->ReadImpl(*dest))
                {
                    return AWS_OP_SUCCESS;
                }

                return AWS_OP_ERR;
            }

            int InputStream::s_GetStatus(aws_input_stream *stream, aws_stream_status *status)
            {
                auto impl = static_cast<InputStream *>(stream->impl);

                *status = impl->GetStatusImpl();
                return AWS_OP_SUCCESS;
            }

            int InputStream::s_GetLength(struct aws_input_stream *stream, int64_t *out_length)
            {
                auto impl = static_cast<InputStream *>(stream->impl);

                int64_t length = impl->GetLengthImpl();

                if (length >= 0)
                {
                    *out_length = length;
                    return AWS_OP_SUCCESS;
                }

                aws_raise_error(AWS_IO_STREAM_READ_FAILED);
                return AWS_OP_ERR;
            }

            void InputStream::s_Acquire(aws_input_stream *stream)
            {
                auto impl = static_cast<InputStream *>(stream->impl);
                impl->AcquireRef();
            }

            void InputStream::s_Release(aws_input_stream *stream)
            {
                auto impl = static_cast<InputStream *>(stream->impl);
                impl->ReleaseRef();
            }

            aws_input_stream_vtable InputStream::s_vtable = {
                InputStream::s_Seek,
                InputStream::s_Read,
                InputStream::s_GetStatus,
                InputStream::s_GetLength,
                InputStream::s_Acquire,
                InputStream::s_Release,
            };

            InputStream::InputStream(Aws::Crt::Allocator *allocator)
            {
                m_allocator = allocator;
                AWS_ZERO_STRUCT(m_underlying_stream);

                m_underlying_stream.impl = this;
                m_underlying_stream.vtable = &s_vtable;
            }

            StdIOStreamInputStream::StdIOStreamInputStream(
                std::shared_ptr<Aws::Crt::Io::IStream> stream,
                Aws::Crt::Allocator *allocator) noexcept
                : InputStream(allocator), m_stream(std::move(stream))
            {
            }

            bool StdIOStreamInputStream::IsValid() const noexcept
            {
                auto status = GetStatusImpl();
                return status.is_valid;
            }

            bool StdIOStreamInputStream::ReadImpl(ByteBuf &buffer) noexcept
            {
                // so this blocks, but readsome() doesn't work at all, so this is the best we've got.
                // if you don't like this, don't use std::input_stream and implement your own version
                // of Aws::Crt::Io::InputStream.
                m_stream->read(reinterpret_cast<char *>(buffer.buffer + buffer.len), buffer.capacity - buffer.len);
                auto read = m_stream->gcount();
                buffer.len += static_cast<size_t>(read);

                if (read > 0 || (read == 0 && m_stream->eof()))
                {
                    return true;
                }

                auto status = GetStatusImpl();

                return status.is_valid && !status.is_end_of_stream;
            }

            StreamStatus StdIOStreamInputStream::GetStatusImpl() const noexcept
            {
                StreamStatus status;
                status.is_end_of_stream = m_stream->eof();
                status.is_valid = static_cast<bool>(*m_stream);

                return status;
            }

            int64_t StdIOStreamInputStream::GetLengthImpl() const noexcept
            {
                auto currentPosition = m_stream->tellg();

                m_stream->seekg(0, std::ios_base::end);
                int64_t retVal = -1;

                if (*m_stream)
                {
                    retVal = static_cast<int64_t>(m_stream->tellg());
                }

                m_stream->seekg(currentPosition);

                return retVal;
            }

            bool StdIOStreamInputStream::SeekImpl(int64_t offset, StreamSeekBasis seekBasis) noexcept
            {
                // very important, otherwise the stream can't be reused after reading the entire stream the first time.
                m_stream->clear();

                auto seekDir = std::ios_base::beg;
                switch (seekBasis)
                {
                    case StreamSeekBasis::Begin:
                        seekDir = std::ios_base::beg;
                        break;
                    case StreamSeekBasis::End:
                        seekDir = std::ios_base::end;
                        break;
                    default:
                        aws_raise_error(AWS_ERROR_INVALID_ARGUMENT);
                        return false;
                }

                using stdOffType = Aws::Crt::Io::IStream::off_type;
                if (offset < std::numeric_limits<stdOffType>::min() || offset > std::numeric_limits<stdOffType>::max())
                {
                    aws_raise_error(AWS_IO_STREAM_INVALID_SEEK_POSITION);
                    return false;
                }

                m_stream->seekg(static_cast<stdOffType>(offset), seekDir);

                return true;
            }
        } // namespace Io
    }     // namespace Crt
} // namespace Aws
