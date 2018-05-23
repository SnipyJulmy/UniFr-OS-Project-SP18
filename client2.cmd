ls
add 10 a
add 20 b
add 30 c
ls
read_k 10
read_k 30
read_k 20
read_v c
read_v b
read_v a
update_kv 30 b
update_kv 20 c
read_k 10
read_k 20
read_k 30
read_v b
read_v c
read_v a
rm_k 20
read_k 20
rm_v b
read_k 30
update_kv 10 d
read_k 10
read_v d
rm_k 10
ls