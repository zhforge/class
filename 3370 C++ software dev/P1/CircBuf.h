
#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <string>
#include <cstddef>
using std::string;

class CircBuf {
	const size_t CHUNK { 8 };
	char* buffer;
	size_t buffer_length = 0;	// total capacity of the buffer
	size_t buffer_size = 0;	// current occupied size of data in the buffer
	size_t buffer_head = 0;	// number representing position of buffer's head
	size_t buffer_tail = 0;	// number representing position of buffer's tail
	void _resize(size_t new_size = 0);
	
public:
	CircBuf(size_t reserve = 0);		// Number of elements you want it to be able to hold to start with.
	~CircBuf();
	size_t	size();
	size_t	capacity();
	
	void		insert(char);
	void		insert (const char*, size_t sz);
	void		insert(const string&);
	char		get();
	string	get(size_t);
	string	flush();	// Returns a string with all the characters, AND shrinks the buffer to zero.	
	string	examine();	
	void		shrink();	// Reduces the unused space in the buffer.
};
#endif
