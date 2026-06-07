# Cách đếm số dòng trong file text bằng PowerShell

Để đếm số dòng của một file text lớn trên Windows PowerShell một cách hiệu quả, bạn có thể sử dụng lệnh `Get-Content` kết hợp với `Measure-Object`.

## Cú pháp

```powershell
Get-Content "<đường_dẫn_tới_file>" | Measure-Object -Line
```

## Ví dụ

Ví dụ để đếm số dòng trong file `raw_logs.txt`:

```powershell
Get-Content "e:\Van Quoc\Antigravity-Project-Code\syslog-analyzer-core\data\raw_logs.txt" | Measure-Object -Line
```

> [!TIP]
> **Tại sao nên dùng cách này?**
> Cách này đọc file theo từng dòng (streaming) thay vì tải toàn bộ nội dung file vào RAM cùng một lúc. Do đó, nó rất phù hợp và an toàn khi làm việc với các file log lớn (hàng trăm ngàn đến hàng triệu dòng) mà không lo bị hết bộ nhớ hay treo máy.
