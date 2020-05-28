#pragma once
#include <string>
#include "Scene.h"

namespace raycore {

struct Reader {
	// Read a single element at cursor and move it.
	template <typename T>
	T read() { T data; read<T>(&data, 1); return data; }
	// Read multiple elements at cursor and move it.
	template <typename T>
	void read(T *data, size_t count) { _read(data, count * sizeof(T)); }

	// Get a pointer at cursor position.
	virtual const uint8_t *data() const = 0;
	// Get the size of the whole buffer.
	virtual size_t size() const = 0;
	// Get the position of the cursor.
	virtual size_t offset() const = 0;
	
protected:
	virtual void _read(char *data, size_t count) = 0;
};

struct MemoryReader : Reader {
	MemoryReader(const uint8_t *bytes, size_t size);
	MemoryReader(const std::vector<uint8_t> &bytes);

	// Simply forward the cursor.
	void skim(size_t count);
	// Get a pointer at cursor position.
	const uint8_t *data() const override;
	// Get the size of the whole buffer.
	size_t size() const override;
	// Get the position of the cursor.
	size_t offset() const override;
	// Set the cursor position at 0.
	void rewind();
private:
	void _read(char *data, size_t size) override;
private:
	const uint8_t *m_bytes; // Bytes of the non-owned buffer.
	size_t m_size;          // Size of the buffer.
	size_t m_offset;        // Current offset position.
};

struct BinaryReader : public Reader {

};

struct TextReader : public Reader {

};

class Loader {
public:
	Loader() {}
	virtual ~Loader() {}

	// Load the file and append it into the scene
	virtual bool load(Reader &reader, prim::Scene &scene) = 0;
};




inline MemoryReader::MemoryReader(const uint8_t *bytes, size_t size) :
	m_bytes(bytes),
	m_size(size),
	m_offset(0)
{
}
inline MemoryReader::MemoryReader(const std::vector<uint8_t> &bytes) :
	m_bytes(bytes.data()),
	m_size(bytes.size()),
	m_offset(0)
{
}
inline void MemoryReader::_read(char *data, size_t size)
{
	if (m_offset + size > m_size)
		throw std::runtime_error("Reading out of bounds");
	memcpy(data, m_bytes + m_offset, size);
	m_offset += size;
}
inline void MemoryReader::skim(size_t count) { m_offset += count; }
inline const uint8_t *MemoryReader::data() const { return m_bytes + m_offset; }
inline size_t MemoryReader::size() const { return m_size; }
inline size_t MemoryReader::offset() const { return m_offset; }
inline void MemoryReader::rewind() { m_offset = 0; }

}


