ls
add 10 a
add 20 b
add 30 c
ls
read_v 10
read_v 30
read_v 20
read_k c
read_k b
read_k a
update_kv 30 b
update_kv 20 c
read_v 10
read_v 20
read_v 30
read_k b
read_k c
read_k a
rm_k 20
read_v 20
rm_v b
read_v 30
update_kv 10 d
read_v 10
read_k d
rm_k 10
ls