# Typinterm
CLI typing trainer sample

## やりたいこと
 - terminalでタイピング
 - 例文１行　入力１行　情報1行の３行構成
 - 全体行数
 - 入力１字単位で正誤評価
 - WPMの計測
 - 正誤評価結果のリアルタイム表示
 - リソース指定
 - typo率の計測
 - 過去実績との競争

## Build & Test
```
$ cmake -B build -S .
$ cmake --build build
$ cd build && ctest
```

## TODO
 - [ ] UIレイアウトデザイン
 - [ ] cursesなど描画ライブラリの選定
