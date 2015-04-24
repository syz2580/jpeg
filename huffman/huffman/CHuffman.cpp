#include <cstdlib>

#include <iostream>
using namespace std;
#include "CHuffman.h"


CHuffman::CHuffman(int src_size, int16 src_map[])
{
	memset(buf, 0, sizeof(buf));
	memset(freq, 0, sizeof(freq));
	for (int i = 0; i < src_size; i++) {
		freq[src_map[i]]++;
	}
	compute_length();
	generate_table();
}



template<typename T>
void CHuffman::swap(T &a, T &b)
{
	T t = a;
	a = b;
	b = t;
}

void CHuffman::heap_insert(int size, int num) {
	int cur_freq, parent_freq;
	buf[size] = num;
	cur_freq = buf[num];

	int index = size;
	parent_freq = buf[buf[index / 2]];
	while (index > 0 && parent_freq > cur_freq) {
		swap(buf[index], buf[index / 2]);
		index /= 2;
		parent_freq = buf[buf[index / 2]];
		cur_freq = buf[buf[index]];

	}
}

void CHuffman::print_buf() {
	for (int i = 0; i < CODE_NUM * 2; i++) {
		cout << buf[i] << ' ';
	}
	cout << endl;
	system("pause");
}

void CHuffman::go_down(int size, int k)
{
	if (k * 2 + 1 >= size) return;
	if (k * 2 + 2 >= size && buf[buf[k]] >= buf[buf[k * 2 + 1]]) {
		swap(buf[k], buf[k * 2 + 1]);
		go_down(size, k * 2 + 1);
	}
	else if (k * 2 + 2 < size) {
		if (buf[buf[k]] >= buf[buf[k * 2 + 1]] && buf[buf[k * 2 + 1]] <= buf[buf[k * 2 + 2]]) {
			swap(buf[k], buf[k * 2 + 1]);
			go_down(size, k * 2 + 1);
		}
		else if (buf[buf[k]] >= buf[buf[k * 2 + 2]] && buf[buf[k * 2 + 2]] <= buf[buf[k * 2 + 1]]){
			swap(buf[k], buf[k * 2 + 2]);
			go_down(size, k * 2 + 2);
		}
	}
}


void CHuffman::compute_length() {
	//compute encoding length, using moffat algorithm
	int heap_size;
	for (int i = CODE_NUM; i < CODE_NUM * 2; i++) {
		buf[i] = freq[i - CODE_NUM];
	}

	//put the index into the heap
	heap_size = 0;
	for (int i = 0; i < CODE_NUM; i++) {
		heap_insert(heap_size++, i + CODE_NUM);
	}
	//print_buf();

	while (heap_size > 1) {
		//put first two nodes to the bottom of the heap
		swap(buf[0], buf[--heap_size]);
		go_down(heap_size, 0);
		swap(buf[0], buf[--heap_size]);
		go_down(heap_size, 0);


		int sum_freq = buf[buf[heap_size + 1]] + buf[buf[heap_size]];
		buf[buf[heap_size + 1]] = heap_size + 1;
		buf[buf[heap_size]] = heap_size + 1;
		buf[heap_size + 1] = sum_freq;


		heap_insert(heap_size++, heap_size);

	}
	//print_buf();


	buf[1] = 0;
	for (int i = 2; i < 2 * CODE_NUM; i++){
		buf[i] = buf[buf[i]] + 1;
	}
	//print_buf();
}

void CHuffman::generate_table() {
	memset(num, 0, sizeof(num));
	
	for (int i = CODE_NUM; i < CODE_NUM * 2; i++) {
		buf[i - CODE_NUM] = num[buf[i]];
		num[buf[i]]++;
	}

	code = 0;
	num[0] = 0;
	
	memset(firstcode, 0, sizeof(firstcode));
	
	for (int i = 1; i <= 32; i++) {
		code <<= 1;
		if (num[i] > 0) {
			firstcode[i] = code;
			code += num[i];
		}
	}

	for (int i = CODE_NUM; i < CODE_NUM * 2; i++) {
		buf[i - CODE_NUM] += firstcode[buf[i]];
	}
}

int16 CHuffman::get_depth(int16 index) {
	return buf[index+CODE_NUM];
}

int16 CHuffman::get_code(int16 index) {
	return buf[index];
}
