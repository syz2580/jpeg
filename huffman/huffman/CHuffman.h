#pragma once
typedef char int8;
typedef short int16;
const int CODE_NUM = 16;
class CHuffman
{
private:
	
	int16 *map;
	int16 buf[CODE_NUM * 2];
	int16 second_line[CODE_NUM];
	unsigned int freq[CODE_NUM];
	unsigned int firstcode[CODE_NUM];
	unsigned int num[33];
	
	template<typename T>
	void swap(T &a, T &b);
	void heap_insert(int size, int num);
	void print_buf();
	void go_down(int size, int k);
	void compute_freq(int size);
	void compute_length();
	void generate_table();
	
	int count_bit(int16 num);

public:
	/*
	* Initialize the huffman table with original data
	* src_size => source table length
	* src_map[]=> source map, to be encoded
	*/
	CHuffman(int src_size, int16 src_map[]);
	~CHuffman();
	/*
	 *Get the depth information
	 *Return a int[16] array
	 */
	int16* get_depth();

	/*
	 * Encode a table into huffman form and save in a file
	 * file_name=> the file name
	 */
	void huffman_encode(char *file_name);
};

