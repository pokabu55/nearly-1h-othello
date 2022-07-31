# nearly1hour-othero
小一時間でゲームを作るより、３章のリバーシを実装。

# 書籍情報
* 小一時間でゲームをつくる ──7つの定番ゲームのプログラミングを体験
* [サポートページ](https://gihyo.jp/book/2022/978-4-297-12745-9/support)

# Ubuntuで表示する場合の設定
* 端末で右クリック→設定
* セルの間隔の幅を1.8倍以上にする（デフォルトは1.0）
* 置き石のないセルの表示を「・」から「□」に変更した

## conio.h 対応
C言語でキーコードを取得するには、getch, getchar関数を使用するが、Linuxにはないのよ…。
https://www.webcyou.com/?p=10625
https://www.web-dev-qa-db-ja.com/ja/c/linux%E3%81%A7c%E3%81%AEgetch%EF%BC%88%EF%BC%89%E9%96%A2%E6%95%B0%E3%82%92%E5%AE%9F%E8%A3%85%E3%81%99%E3%82%8B%E6%96%B9%E6%B3%95%E3%81%AF%EF%BC%9F/969438634/
