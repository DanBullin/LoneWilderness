/*! \file bufferLayout.h
*
* \brief A buffer layout
*
* \author DMU Course material
*
*/
#ifndef BUFFERLAYOUT_H
#define BUFFERLAYOUT_H

#include "independent/core/common.h"
#include "independent/rendering/shaders/shaderDataType.h"

namespace Engine
{
	/*! \class VertexBufferElement
	* \brief Holds data about a single element in a vertex buffer layout
	*/
	class VertexBufferElement
	{
	public:
		ShaderDataType m_dataType; //!< The data type of the element
		uint32_t m_size; //!< The size of memory of the data type
		uint32_t m_offset; //!< The offset in memory from the beginning
		bool m_normalized; //!< Is the element's data normalized
		uint32_t m_instanceDivisor; //!< The instance divisor
		VertexBufferElement() = default; //!< Default constructor
		VertexBufferElement(const ShaderDataType type, const bool normalized = false, const uint32_t instanceDivisor = 0) : m_dataType(type), m_size(SDT::getSize(type)),
			m_offset(0), m_normalized(normalized), m_instanceDivisor(instanceDivisor) {}; //!< Constructor
			/*!< \param type a const ShaderDataType - The data type of the element
				 \param normalized a const bool - Is the element normalized
				 \param instanceDivisor a const uint32_t - The instance divisor */
	};

	/*! \class UniformBufferElement
	* \brief Holds data about a single element in a uniform buffer layout
	*/
	class UniformBufferElement
	{
	public:
		std::string m_name; //!< Name of the uniform buffer element
		ShaderDataType m_dataType; //!< The data type of the element
		uint32_t m_size; //!< The size of memory of the data type
		uint32_t m_offset; //!< The offset in memory from the beginning
		UniformBufferElement() = default; //!< Default constructor
		UniformBufferElement(const std::string& name, ShaderDataType type) : m_name(name), m_dataType(type), m_size(SDT::getST140Alignment(type)),
			m_offset(0) {}; //!< Constructor
			/*!< \param name a const std::string& - The name of the uniform buffer element
				 \param type a ShaderDataType - The data type of the element */
	};

	/*! \class BufferLayout
	* \brief An abstraction of a buffer layout
	*/
	template <class T>
	class BufferLayout
	{
	private:
		std::vector<T> m_elements; //!< The buffer elements
		uint32_t m_stride = 0; //!< The stride in bytes of the buffer line
		void calculateBufferLayout(); //!< Calculate the buffer offset and strides based on elements 
	public:
		BufferLayout<T>() {}; //!< Default Constructor
		BufferLayout<T>(const std::initializer_list<T>& element, uint32_t stride = 0) : m_elements(element), m_stride(stride) { calculateBufferLayout(); } //!< Constructor
			/*!< \param element a const std::initializer_list<T>& - The elements to add to the list of elements
				 \param stride a uint32_t - The stride of the layout */
		inline const uint32_t getStride() const { return m_stride; } //!< Get the stride of the layout
			/*!< \return a const uint32_t - The stride of the buffer layout */
		inline const std::vector<T>& getElements() const { return m_elements; } //!< Get the elements in the buffer layout
			/*!< \return a const std::vector<T>& - The elements in the buffer layout */
		void addElement(const T element); //!< Add an element to the layout
		void recalculateStride(const uint32_t overrideStride = 0); //!< Recalculate the stride
		void calculateUniformBufferLayout(); //!< Calculate uniform buffer layout using std140 memory layout
		inline typename std::vector<T>::iterator begin() { return m_elements.begin(); } //!< Get the begin iterator for the elements
			/*!< \return a std::vector<T>::iterator - The begin iterator of the elements list */
		inline typename std::vector<T>::iterator end() { return m_elements.end(); } //!< Get the end iterator for the elements
			/*!< \return a std::vector<T>::iterator - The end iterator of the elements list */
		inline typename std::vector<T>::const_iterator begin() const { return m_elements.begin(); } //!< Get the const begin iterator for the elements
			/*!< \return a std::vector<T>::const_iterator - The begin const iterator of the elements list */
		inline typename std::vector<T>::const_iterator end() const { return m_elements.end(); } //!< Get the const end iterator for the elements
			/*!< \return a std::vector<T>::const_iterator - The end const iterator of the elements list */
	};

	template <class T>
	//! calculateBufferLayout()
	void BufferLayout<T>::calculateBufferLayout()
	{
		uint32_t offset = 0;

		// Calculate the offset for each element and the overall stride
		for (auto& element : m_elements)
		{
			element.m_offset = offset;
			offset += element.m_size;
		}

		if (m_stride == 0) { m_stride = offset; }
	}

	template<class T>
	//! calculateUniformBufferLayout()
	void BufferLayout<T>::calculateUniformBufferLayout()
	{
		uint32_t offset = 0;

		// Calculate the offset for each element and the overall stride
		for (auto& element : m_elements)
		{
			uint32_t remainder = 0;

			if(offset != 0)
				remainder = offset % SDT::getAlignmentOffset(element.m_dataType);
			
			if (remainder == 0)
				element.m_offset = offset;
			else
				element.m_offset = offset - remainder + SDT::getAlignmentOffset(element.m_dataType);

			offset = element.m_offset + element.m_size;
		}

		if (m_stride == 0) { m_stride = offset; }
	}

	template <class T>
	//! addElement()
	/*!
	\param element a T - The buffer element to add to the list
	*/
	void BufferLayout<T>::addElement(T element)
	{
		// Add the element to the back of the list and recalculate the buffer layout
		m_elements.push_back(element);
		calculateBufferLayout();
	}

	template <class T>
	//! recalculateStride()
	/*!
	\param overrideStride a const uint32_t - The overriding stride value
	*/
	void BufferLayout<T>::recalculateStride(const uint32_t overrideStride)
	{
		if (overrideStride != 0)
			m_stride = overrideStride;
		else
		{
			uint32_t stride = 0;

			// Calculate the stride by adding all element sizes
			for (auto& element : m_elements)
				stride += element.m_size;

			m_stride = m_elements.back().m_offset + m_elements.back().m_size;
		}
	}

	using VertexBufferLayout = BufferLayout<VertexBufferElement>; //!< Type alias
	using UniformBufferLayout = BufferLayout<UniformBufferElement>; //!< Type alias
}
#endif