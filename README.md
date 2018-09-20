# machino_os

まちのOS


## by_date

* 日付指定でデータをフィルタリングする

### build
オプション```-std=c++11```が有効なg++が必要

```sh
make by_date
```

### usage
\<date\>のフォーマットは```YYYY-MM-DD```

```
./by_date ../data/ <date>
```


## recent

* 範囲(タイムスタンプ)指定でデータをフィルタリングする

### build
オプション```-std=c++11```が有効なg++が必要

```sh
make recent
```

### usage
```
cat ../data/thp | ./recent <timestamp_from> <timestamp_until>
```
