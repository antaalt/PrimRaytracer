#pragma once
#include <string>
#include "Scene.h"

namespace raycore {

struct Reader {
	Reader(const uint8_t *bytes, size_t size);
	Reader(const std::vector<uint8_t> &bytes);

	// Read a single element at cursor and move it.
	template <typename T>
	T read();
	// Read multiple elements at cursor and move it.
	template <typename T>
	void read(T *data, size_t count);

	// Simply forward the cursor.
	void skim(size_t count);
	// Get a pointer at cursor position.
	const uint8_t *data() const;
	// Get the size of the whole buffer.
	size_t size() const;
	// Get the position of the cursor.
	size_t offset() const;
	// Set the cursor position at 0.
	void rewind();
private:
	const uint8_t *m_bytes; // Bytes of the non-owned buffer.
	size_t m_size;          // Size of the buffer.
	size_t m_offset;        // Current offset position.
};

struct Writer {

};

class Loader {
public:
	Loader() {}
	virtual ~Loader() {}

	// Load the file and append it into the scene
	virtual bool load(Reader &reader, prim::Scene &scene) = 0;
	// Write to a file the scene
	virtual bool write(Writer &writer, const prim::Scene &scene) = 0;
};




inline Reader::Reader(const uint8_t *bytes, size_t size) :
	m_bytes(bytes),
	m_size(size),
	m_offset(0)
{
}
inline Reader::Reader(const std::vector<uint8_t> &bytes) :
	m_bytes(bytes.data()),
	m_size(bytes.size()),
	m_offset(0)
{
}
template <typename T>
inline T Reader::read()
{
	T data;
	read(&data, 1);
	return data;
}
template <typename T>
inline void Reader::read(T *data, size_t count)
{
	if (m_offset + count * sizeof(T) > m_size)
		throw parse_error("Reading out of bounds");
	memcpy(data, m_bytes + m_offset, count * sizeof(T));
	m_offset += count * sizeof(T);
}
inline void Reader::skim(size_t count) { m_offset += count; }
inline const uint8_t *Reader::data() const { return m_bytes + m_offset; }
inline size_t Reader::size() const { return m_size; }
inline size_t Reader::offset() const { return m_offset; }
inline void Reader::rewind() { m_offset = 0; }

}


