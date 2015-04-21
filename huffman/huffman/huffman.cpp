#include <iostream>
using namespace std;
typedef char int8;
const int CODE_NUM = 8;
int8 map[10000];
unsigned int outmap[10000];
unsigned int buf[CODE_NUM * 2];
unsigned int freq[CODE_NUM];
unsigned int firstcode[CODE_NUM];
unsigned int num[33];

void swap(int &a, int &b)
{
	int t = a;
	a = b;
	b = t;
}

void heap_insert(int size, int num) {
	int cur_freq, parent_freq;
	buf[size] = num;
	cur_freq = buf[num];

	int index = size;
	parent_freq = buf[ buf[index / 2] ];
	while (index > 0 && parent_freq > cur_freq) {
		swap(buf[index], buf[index / 2]);
		index /= 2;
		parent_freq = buf[buf[index / 2]];
		cur_freq = buf[buf[index]];
		
	}
}

void print_buf() {
	for (int i = 0; i < CODE_NUM * 2; i++) {
		cout << buf[i] << ' ';
	}
	cout << endl;
	system("pause");
}

void go_down(int size, int k)
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

void compute_freq(int size) {
	memset(freq, 0, sizeof(freq));
	for (int i = 0; i < size; i++) {
		freq[map[i]]++;
	}
}

void compute_length() {
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
	print_buf();

	while (heap_size > 1) {
		//put first two nodes to the bottom of the heap
		swap(buf[0], buf[--heap_size]);
		go_down(heap_size,0);
		swap(buf[0], buf[--heap_size]);
		go_down(heap_size, 0);
	

		int sum_freq = buf[buf[heap_size + 1]] + buf[buf[heap_size]];
		buf[buf[heap_size + 1]] = heap_size + 1;
		buf[buf[heap_size]] = heap_size + 1;
		buf[heap_size + 1] = sum_freq;
		

		heap_insert(heap_size++, heap_size);

	}
	print_buf();
	

	buf[1] = 0;
	for (int i = 2; i < 2 * CODE_NUM; i++){
		buf[i] = buf[buf[i]] + 1;
	}
	print_buf();
}

void generate_table() {
	memset(num, 0, sizeof(num));
	for (int i = CODE_NUM; i < CODE_NUM * 2; i++) {
		buf[i - CODE_NUM] = num[buf[i]]++;
	}

	int code = 0;
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

void encode_huffman(char *file_dir) {
	FILE *fp;
	fopen_s(&fp, file_dir, "wb");
	int8 a = 3;
	fwrite(&a, 1, 1, fp);
	fclose(fp);
}

int main(){
	for (int i = 0; i < 10; i++){
		map[i] = 0;
	}
	map[11] = 2;
	map[10] = 1;
	for (int i = 12; i < 23; i++){
		map[i] = 3;
	}
	map[23] = 4;
	map[24] = 5;
	for (int i = 25; i < 33; i++){
		map[i] = 6;
	}
	for (int i = 33; i < 38; i++){
		map[i] = 7;
	}
	compute_freq(38);
	compute_length();
	generate_table();
	print_buf();

	encode_huffman("D:\\a.bin");
	
}