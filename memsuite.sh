objdump -d $1 -Mintel | awk -v RS= '/^[[:xdigit:]].*<main>/'
