#pragma once
typedef char int8;
const int CODE_NUM = 16;
class CHuffman
{
private:
	
	int8 map[10000];
	unsigned int outmap[10000];
	unsigned int buf[CODE_NUM * 2];
	unsigned int freq[CODE_NUM];
	unsigned int firstcode[CODE_NUM];
	unsigned int num[33];
	int depth[16];
	
	template<typename T>
	void swap(T &a, T &b);
	void heap_insert(int size, int num);
	void print_buf();
	void go_down(int size, int k);
	void compute_freq(int size);
	void compute_length();
	void generate_table();
	

public:
	CHuffman();
	~CHuffman();
	/*
	 *Get the depth information
	 *Return a int[16] array
	 */
	int* get_depth();

	/*
	 * Encode a table into huffman form and save in a file
	 * src_size => source table length
	 * src_map[]=> source map, to be encoded
	 * file_name=> the file name
	 */
	void huffman_encode(int src_size,int src_map[],char *file_name);
};

