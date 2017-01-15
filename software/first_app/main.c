#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "altera_avalon_performance_counter.h"
#include <system.h>
#include "sys/alt_cache.h"
#include "alt_types.h"

#define FILTER_SIZE 7
#define FILTER_FRACTIONAL_BITS 11
#define PADDING_SIZE 3

int main() {

	FILE* file; // file to be read

	unsigned int i, j, k, p; // iterators

	short * read_value_int;

	float** filter;

	unsigned long *width, *height; // image dims
	unsigned char **image;
	unsigned char *image_buf;

	float tmp;

	/* read filter */
	file = fopen("/mnt/host/filter_coeffs.bin", "rb");

	filter = (float**) malloc(sizeof(float*) * FILTER_SIZE);
	read_value_int = (short*) malloc(4);
	for (i = 0; i < FILTER_SIZE; i++) {
		filter[i] = (float*) malloc(sizeof(float) * FILTER_SIZE);
		for (j = 0; j < FILTER_SIZE; j++) {
			fread(read_value_int, 2, 1, file);
			filter[i][j] = (float) *read_value_int / pow(2, FILTER_FRACTIONAL_BITS);
		}
	}

	fclose(file);

	PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
	PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);

	/* read image */

	file = fopen("/mnt/host/cam.bin", "rb");

	width = (unsigned long*) malloc(4);
	fread(width, 4, 1, file);

	height = (unsigned long*) malloc(4);
	fread(height, 4, 1, file);

	image_buf = (unsigned char*) malloc(sizeof(unsigned char) * (*height) * (*width));
	fread(image_buf, 1, (*height) * (*width), file);

	fclose(file);

	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1); // testing image padding
	image = (unsigned char**) malloc(sizeof(unsigned char*) * (*width + 2 * PADDING_SIZE));
	for (i = 0; i < *width + 2 * PADDING_SIZE; i++) {
		image[i] = (unsigned char*) malloc(sizeof(unsigned char) * (*height + 2 * PADDING_SIZE));
		for (j = 0; j < *height + 2 * PADDING_SIZE; j++) {
			if (j < PADDING_SIZE || j >= *height + PADDING_SIZE || i < PADDING_SIZE || i >= *width + PADDING_SIZE) {
				image[i][j] = 0;
			} else {
				image[i][j] = image_buf[(i - PADDING_SIZE) * (*width) + j - PADDING_SIZE];
			}
		}
	}

	PERF_END(PERFORMANCE_COUNTER_0_BASE, 1); // end testing image padding

	/* perform filtering */

	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 2); // testing image filtering
	for (i = 0; i < *width; i++) {
		for (j = 0; j < *height; j++) {
			tmp = 0;
			for (k = 0; k < FILTER_SIZE; k++) {
				for (p = 0; p < FILTER_SIZE; p++) {
					tmp += filter[k][p] * image[i + k][j + p];
				}
			}
			image_buf[i * (*width) + j] = (unsigned char) tmp;
		}
		printf("%d\n", i);
	}
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 2); // end testing image filtering

	file = fopen("/mnt/host/output_image.bin", "wb");
	fwrite(width, 4, 1, file);
	fwrite(height, 4, 1, file);
	fwrite(image_buf, 1, (*width) * (*height), file);
	fclose(file);

	perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE,
			alt_get_cpu_freq(),
			2,
			"Image zero padding",
			"Applying filter to the image");

	free(image);
	free(image_buf);
	free(filter);
	free(height);
	free(width);
	free(read_value_int);

	return 0;
}
