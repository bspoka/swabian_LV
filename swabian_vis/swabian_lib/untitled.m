clear
fpath = 'C:\Users\bspoka\Google Drive\Programming\c_projects\swabian_lib\irf_405nm.photons';

fid = fopen(fpath, 'r');

d = fread(fid, [3, 1E6], 'uint64');
fclose(fid);

%%
d3 = d(3, :);

bins = linspace(min(d3), max((d3)), 2048);

[a, b]  = histcounts(d3, bins);

plot(b(1:end-1), a)
set(gca, 'yscale', 'log')

%%

fpath = '/Users/bspoka/Google Drive/Programming/c_projects/swabian_lib/test1.photons';

fid = fopen(fpath, 'r');

d = fread(fid, [2, 1E6], 'uint64');
fclose(fid);

%%
clear all;

sync_before = zeros(1, 20);
sync_before(1:2:end) = 1;
sync_before(2:2:end) = 2;
sync_before([9, 10]) = 1;

stamps = 1:20;

buffer = [sync_before; stamps];
%%
path = '/Users/bspoka/Google Drive/Programming/c_projects/swabian_lib/test_after.photons';
fileID = fopen(path,'w');
fwrite(fileID,uint64(buffer),'uint64');
fclose(fileID);