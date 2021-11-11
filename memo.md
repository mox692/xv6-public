* 11/7
  * makefile読んで、buildの構成をざっくり把握した
    * 構造としては、
      * kernel
      * bootloader
      * fs
    * みたいな構成になってそう
* next
  * bootloader(bootasm.S bootmain.c)あたりからのんびり読んでいく.
    * ざっと見た感じ、legacy bios(16bit mode)っぽいw

* 11/9
  * bootasm, bootmain読んだ
  * 次からkernel読めそう?
    * entry.Sとかいうのもあるから、先にそっち？
    * もっかいmakefile見てbuildの手順確認する.

* 11/11
  * main.cに入った瞬間、むずすぎた..
    * この辺はblogとかを読みつつ進めた方が良さげ.
  * linker scriptをざっくり読んだ(ここはそこまでむずくなかった.)


### 参考文献
* boot, pagingの設定(main.cのはじめあたり)
  * https://yohei.codes/ja/post/xv6-memory-2/
  * 