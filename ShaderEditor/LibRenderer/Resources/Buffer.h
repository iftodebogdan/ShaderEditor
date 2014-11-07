#ifndef BUFFER_H
#define BUFFER_H

namespace LibRendererDll
{
	//This is the base class from which the VertexBuffer and IndexBuffer classes are derived from.
	class Buffer
	{
	public:
		//Usage options that identify how resources are to be used.
		enum BufferUsage
		{
			BU_STATIC,			//Informs the system that the application writes only to the vertex buffer.
			BU_DYNAMIC,			//Set to indicate that the vertex buffer requires dynamic memory use.
			BU_RENDERTAGET,		//The resource will be a render target.
			BU_DEPTHSTENCIL,	//The resource will be a depth stencil buffer.

			BU_MAX
		};

		enum BufferLocking
		{
			BL_READ_ONLY,	//The application will not write to the buffer.
			BL_WRITE_ONLY,
			BL_READ_WRITE,

			BL_MAX
		};

		const	unsigned int	GetElementCount() const;
				void			SetElementCount(unsigned int numElements);
		const	unsigned int	GetElementSize() const;
		const	BufferUsage		GetUsage() const;
		const	unsigned int	GetSize() const;
		const	void*			GetData() const;

	protected:
						Buffer(unsigned int elementCount, unsigned int elementSize, BufferUsage usage);
		virtual			~Buffer();

		unsigned int	m_nElementCount;
		unsigned int	m_nElementSize;
		BufferUsage		m_eBufferUsage;
		unsigned int	m_nSize;
		void*			m_pData;
	};
}

#endif //BUFFER_H