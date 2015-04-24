#pragma once
typedef char int8;
typedef unsigned short int16;
const int CODE_NUM = 256;
class CHuffman
{
private:
	
	unsigned int buf[CODE_NUM * 2];
	//int16 second_line[CODE_NUM];
	unsigned int freq[CODE_NUM];
	unsigned int firstcode[CODE_NUM];
	unsigned int num[257];
	int code;
	
	template<typename T>
	void swap(T &a, T &b);
	void heap_insert(int size, int num);
	void print_buf();
	void go_down(int size, int k);
	void compute_length();
	void generate_table();
	
public:
	/*
	* Initialize the huffman table with original data
	* src_size => source table length
	* src_map[]=> source map, to be encoded
	*/
	CHuffman(int src_size, int16 src_map[]);
	/*
	 * Get the depth of a certain index
	 */
	int16 get_depth(int16 index);
	/*
	 * Get the code of a certain index
	 */
	int16 get_code(int16 index);
};

