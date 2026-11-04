# 题库训练器

基于 Qt 6 Widgets 的选择题训练器，支持单选题、多选题和判断题。

## 功能

- 导入 `.xlsx` / `.xlsm` 题库
- 单选、多选、判断三种题型
- 训练题目随机抽取且不重复
- 根据题库数量限制训练题数
- 题库导入、重命名和删除
- 提交答案后显示正确答案和解析
- 错误答案显示红色，正确选项显示绿色
- 完成训练后显示错题列表

## Excel 格式

Excel 至少需要包含：

- `题目`
- `答案`

单选题和多选题还需要选项列，可以使用 `A`、`B`、`C`、`D`、`E`，也可以使用 `选项A`、`选项B` 等表头。判断题可以只填写题目和答案，答案支持“正确/错误”或 `A/B`。

## 开发环境

- Qt 6.11.0 MinGW 64-bit
- CMake
- MinGW 13.1.0
- Windows 64 位

## 构建

在 Qt Creator 中打开项目，选择 Qt 6 MinGW 64-bit Kit，配置为 Release 后构建。

也可以在 Qt MinGW 环境中执行：

```powershell
cmake -S . -B build/release -G "MinGW Makefiles" `
  -DCMAKE_PREFIX_PATH=H:/qt/6.11.0/mingw_64 `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_C_COMPILER=H:/qt/Tools/mingw1310_64/bin/gcc.exe `
  -DCMAKE_CXX_COMPILER=H:/qt/Tools/mingw1310_64/bin/g++.exe
cmake --build build/release --parallel 1
```

## 打包绿色版

```powershell
New-Item -ItemType Directory -Force dist/题库训练器
Copy-Item build/release/train_for_test.exe dist/题库训练器/ -Force
& H:/qt/6.11.0/mingw_64/bin/windeployqt.exe `
  --release --compiler-runtime --no-translations `
  dist/题库训练器/train_for_test.exe
Compress-Archive -Path dist/题库训练器 `
  -DestinationPath dist/题库训练器-绿色版.zip -Force
```

发布时需要上传整个 `题库训练器-绿色版.zip`，不能只上传 exe。
