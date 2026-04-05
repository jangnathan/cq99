#include <stdio.h>

void bubble(int *in, int len) {
	for (int i = 0; i < len - 1; i++) {
		if (in[i] > in[i + 1]) {
			int temp = in[i];
			in[i] = in[i + 1];
			in[i + 1] = temp;
		}
	}
}

void print_int_array(int *nums, int len) {
	for (int i = 0; i < len - 1; i++) {
		printf("%d, ", nums[i]); 
	}
	printf("%d", nums[len - 1]);
}

int main() {
	int nums[] = {0, 5, 2, 9, 6, 10};

	int len = sizeof(nums) / sizeof(int);
	bubble(nums, len);
	print_int_array(nums, len);
}
