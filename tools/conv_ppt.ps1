$src = "D:\project\LGLS\Renewal\260818_ECS-분석-PLC사양및시나리오_LG생명과학_V1.1.ppt"
$dst = "C:\Users\USER\AppData\Local\Temp\claude\D--project-LGLS-Renewal\acbf154d-f43c-4419-af82-1b94e3fa8c7d\scratchpad\plc_orig.pptx"
if (Test-Path $dst) { Remove-Item $dst -Force }
$ppt = New-Object -ComObject PowerPoint.Application
$pres = $ppt.Presentations.Open($src, $true, $false, $false)
$pres.SaveAs($dst, 24)   # ppSaveAsOpenXMLPresentation
Write-Output ("slides: " + $pres.Slides.Count)
$pres.Close(); $ppt.Quit()
Write-Output ("saved: " + $dst)
