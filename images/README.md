## WingGifEditor

&emsp;&emsp;本软件是基于 QT 编写的 GIF 编辑器，采用 C++ 进行开发，目的是让 Deepin 上具有简单易用的 GIF 编辑器。`Windows`上有一个十分强大都既是 GIF 录屏工具又是强大 GIF 编辑器`ScreenToGif`。但 Linux 上只有强大的 GIF 录屏工具，但没有简单好用满足基本编辑需求的 GIF 编辑器。于是乎，我给开发了一个简易的 GIF 编辑器，它具有基本的编辑功能，简单方便。

&emsp;&emsp;有关本软件提建议的时候不要天马行空，本软件的定位是提供简单易用满足基本需求的 GIF 编辑器，并不是像`ScreenToGif`，一是没有相关基础知识，自己一个人搞不动；二是没必要，我不想把这个软件搞个大而全，配合`Gimp`等优秀的图像编辑器作为辅助，来编辑 GIF 图像绰绰有余了。

### 软件依赖

&emsp;&emsp;本软件基于`ImageMagick`，它是一个强大的图片处理库，这个是官网介绍：

> Use ImageMagick® to create, edit, compose, or convert digital images. It can read and write images in a variety of formats (over 200) including PNG, JPEG, GIF, WebP, HEIC, SVG, PDF, DPX, EXR and TIFF. ImageMagick can resize, flip, mirror, rotate, distort, shear and transform images, adjust image colors, apply various special effects, or draw text, lines, polygons, ellipses and Bézier curves.

&emsp;&emsp;功能挺强大的，似乎对于该软件有点大材小用，不过我会尽量充分利用该库，实现与 GIF 编辑相关的操作。

### 编译安装

&emsp;&emsp;由于本软件依赖`ImageMagick`和`DTK`，`DTK`直接在深度应用商店下载就行了，前者你需要在终端输入：

```bash
sudo apt install graphicsmagick-libmagick-dev-compat
```

&emsp;&emsp;安装完毕后，打开项目，你就可以直接编译该软件了。

### 参与贡献

1. 如果您有想参与本软件代码开发递交，请在 pull request 联系我。
2. 本项目支持捐助，如有意愿请到本仓库通过微信或者支付宝的方式进行，一瓶水的价钱足以提高我的维护该项目的热情，感谢大家的支持。
3. 如果您想提交修复或者增进程序的代码，请在 pull request 递交。
4. 任何成功参与代码 Bug 修复以及增进程序功能的同志和 Sponsor ，都会在本仓库 ReadMe 和附属说明文件中体现，您如果是其中之一，本人可以按照您合理的意愿来进行说明。
  

**加入我们并不意味着就是代码的维护，你可以选择下列一项或多项进行参与：**

1. 代码维护：实现新功能或修复 BUG ，对代码进行维护和升级。
2. 文档编辑：主要是接口文档和教程需要撰写编辑，这很重要。
3. 参与讨论：主要是讨论本项目未来发展和方向等。
4. 编写插件：一起增强该软件的功能。

### 协议

&emsp;&emsp;本软件仓库遵循`AGPL-3.0`协议，不得将本软件用于协议之外的用途。

## 有关仓库

- Gitea : https://code.gitlink.org.cn/wingsummer/WingGifEditor
- Gitee : https://gitee.com/wingsummer/wing-gif-editor
- Github : https://github.com/Wing-summer/WingGifEditor