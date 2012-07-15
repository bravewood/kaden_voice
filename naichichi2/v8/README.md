V8 - JavaScript Engine
===========================================================

[V8][id] is Google's open source JavaScript engine.

## これは何？
Naichichi2にv8を組み込むためのファイル群があります。
このディレクトリにあるファイル群はv8のリポジトリからチェックアウトしたファイルからビルドに必要なものを抜き出したものです。
最新のv8コードではないことに注意してください。


## v8をどのように使うか？
家電を制御するルールをJavaScriptで書いた場合そのルールを解釈実行するためにv8を使います。





## ビルドする

v8をビルドするのに必要な手順を説明します。

ここに書いてあることです。
http://d.hatena.ne.jp/bravewood/20120617


環境

OS: windows vista or 7

IDE: VC++ 2010 Express


外部プログラム

python 2.7.3





1.all.slnを開く

v8/build/all.sln
をダブルクリック


2.ビルド

F7でソリューションをビルドします




## 最新のv8にする

最新のv8コードに合わせたい場合は手作業でファイルを上書き/追加してください。



[id]: http://code.google.com/p/v8/
