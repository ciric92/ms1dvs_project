fp = fopen('avg.bin', 'wb');

fractional_bits = 11;

filter_mask = [1 1 1 1 1 1 1;
               1 1 1 1 1 1 1;
               1 1 1 1 1 1 1;
               1 1 1 1 1 1 1;
               1 1 1 1 1 1 1;
               1 1 1 1 1 1 1;
               1 1 1 1 1 1 1]/49;

filter_mask_fixed_point = int16(filter_mask*(2^fractional_bits));

fwrite(fp, filter_mask_fixed_point', 'int16');

fclose(fp);

fp = fopen('log.bin', 'wb');

fractional_bits = 11;

filter_mask = [0  0  0  0  0  0  0;
               0  0  0 -1  0  0  0;
               0  0 -1 -2 -1  0  0;
               0 -1 -2 16 -2 -1  0;
               0  0 -1 -2 -1  0  0;
               0  0  0 -1  0  0  0;
               0  0  0  0  0  0  0];

filter_mask_fixed_point = int16(filter_mask*(2^fractional_bits));

fwrite(fp, filter_mask_fixed_point', 'int16');

fclose(fp);


fp = fopen('sharp.bin', 'wb');

fractional_bits = 11;

filter_mask = [0  0  0  0  0  0  0;
               0  0  0  0  0  0  0;
               0  0 -1 -1 -1  0  0;
               0  0 -1 17 -1  0  0;
               0  0 -1 -1 -1  0  0;
               0  0  0  0  0  0  0;
               0  0  0  0  0  0  0]/9;

filter_mask_fixed_point = int16(filter_mask*(2^fractional_bits));

fwrite(fp, filter_mask_fixed_point', 'int16');

fclose(fp);