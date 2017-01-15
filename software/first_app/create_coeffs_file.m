fp = fopen('filter_coeffs.bin', 'wb');

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