echo "Compress data folder"

cd data
rm -rf *.gz

for file in *.html; do gzip -c --no-name --best "$file" > "$file.gz"; done
for file in *.txt; do gzip -c --no-name --best "$file" > "$file.gz"; done
for file in *.js; do gzip -c --no-name --best "$file" > "$file.gz"; done
for file in *.css; do gzip -c --no-name --best "$file" > "$file.gz"; done
