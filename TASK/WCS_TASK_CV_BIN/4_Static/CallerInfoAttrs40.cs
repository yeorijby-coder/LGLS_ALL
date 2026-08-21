// [LGLS 2026-08-21] .NET 4.0 프로젝트(IO_TASK / HOST_TASK)용 Caller Info 어트리뷰트 폴리필.
//   컴파일러는 이름으로 인식하므로 자체 정의만 있으면 [CallerFilePath]/[CallerMemberName]이 동작한다.
//   ⚠ .NET 4.5+ 프로젝트(WCS_TASK_CV 등)에는 include 하지 말 것 (mscorlib 정의와 중복).
namespace System.Runtime.CompilerServices
{
    [AttributeUsage(AttributeTargets.Parameter, Inherited = false)]
    public sealed class CallerMemberNameAttribute : Attribute { }

    [AttributeUsage(AttributeTargets.Parameter, Inherited = false)]
    public sealed class CallerFilePathAttribute : Attribute { }

    [AttributeUsage(AttributeTargets.Parameter, Inherited = false)]
    public sealed class CallerLineNumberAttribute : Attribute { }
}
