clbiff
=====

[![version](http://img.shields.io/github/tag/sasairc/clbiff.svg?style=flat&label=version)](https://github.com/sasairc/clbiff/releases)
[![license](https://img.shields.io/badge/License-WTFPL2-blue.svg?style=flat)](http://www.wtfpl.net/txt/copying/)
[![issues](http://img.shields.io/github/issues/sasairc/clbiff.svg?style=flat)](https://github.com/sasairc/clbiff/issues)
[![build](https://img.shields.io/travis/sasairc/clbiff.svg?style=flat)](https://travis-ci.org/sasairc/clbiff)

新着メールの到着を任意のコマンドにてお知らせする何か。		
普通のファイルも監視できます。

## install

```shellsession
% make PREFIX=/foo/bar	# デフォルトでは、PREFIX=/usr/localです
# make install PREFIX=/foo/bar
```

## zshでの補完

`$(PREFIX)/share/clbiff/compdef/_clbiff.zsh`を`_clbiff`として、`$fpath`の通ったディレクトリにコピーして下さい。

## Option

* `-i, --interval` option  
	秒単位で時間を確認周期の設定をします。デフォルト値は30秒です。
* `-f, --file` option  
	確認するメールボックスを指定します。  
	デフォルトではシェル環境変数`$MAIL`を参照し、Maildir(`$MAIL/new`)およびMH形式(`$MAIL/inbox`)をサポートします。  
	他環境変数と同様、`:`区切りでの複数記述もサポートしますが、監視対象になるのは優先順位の一番高いディレクトリのみです。

* `-c, --command` option  
	実行するコマンドを指定します。オプション及び、複数コマンド間パイプのみのサポートです。		
	デフォルトではCDドライブを排出する`eject`です。 参考: [cdbiff](http://0xcc.net/cdbiff/)

* `-q, --quiet` option  
	メッセージの抑制をします。（デフォルト）

* `-v, --verbose` option  
	何かが起こる度にメッセージを出力するようにします。（鬱陶しい）

## Basic Usage

```shellsession
% clbiff --file="$HOME/Mail/inbox" --command="yasuna" -i 60 &
[1] 10367
.
.
.
よし・・・これでって・・・あっ！
.
.
.

% kill 10367	# SIGINTやSIGTERMを受信すると終了処理をします
```

## Additional Usage

``` shellsession
% clbiff --file="/mnt/ramdisk/seamonkey/cache2/entries" --command="xterm" -i 1 &
```
お察し下さい

## ~/.clbiffrc

`~/.clbiffrc`が存在する場合は、記述された設定が読み込まれます。

```shellsession
% cat <<EOF > ~/.clbiffrc
command:yasuna | n_cipher encode | nkf -f80 | clangsay --think
file:/mnt/ramdisk/seamonkey/cache2/entries
interval:1
verbose:1
EOF
% clbiff &
clbiff 0.7
pid      = 22453
file     = /mnt/ramdisk/seamonkey/cache2/entries
interval = 1 sec
command  = yasuna | n_cipher encode | nkf -f80 | clangsay --think
```

## Additional Infomation

`config.h`で規定の動作を変更できます。	
使い道は分かりませんが、`WITH_USLEEP`を有効にするとマイクロ秒単位で確認できたり・・・
	
## License

[WTFPL version 2](http://www.wtfpl.net/txt/copying/)

## Author

sasairc (https://github.com/sasairc)
