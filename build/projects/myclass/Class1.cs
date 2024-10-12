using System;
using System.Runtime.InteropServices;

namespace myclass;

public class Transform
{

    [DllImport("/home/delstre-/cnt/prj/vxl/build/projects/myclass/libs.so", EntryPoint = "GetX")]
    public static extern IntPtr GetPosition(IntPtr objectPtr);

    public IntPtr GetXInstance(IntPtr objectPtr)
    {
        return GetPosition(objectPtr);
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct MyStruct
{
    public double x;
    public int y;
    public int z;
}

public class Object
{
    [DllImport("/home/delstre-/cnt/prj/vxl/build/projects/myclass/libs.so", EntryPoint = "GetTransform")]
    public static extern IntPtr GetTransform(IntPtr objectPtr);

    public IntPtr ObjectPtr;

    public IntPtr GetTransformInstance()
    {
        IntPtr transformPtr = GetTransform(ObjectPtr);
        return transformPtr;
    }

    public Object(IntPtr objectPtr)
    {
        ObjectPtr = objectPtr;
    }

}

public class Class1 {
    public void Update() {
        var obj = GetParent();
        if (obj != null)
        {
            var transform = new Transform();
            var x = transform.GetXInstance(ParentPtr);
            MyStruct myStruct = Marshal.PtrToStructure<MyStruct>(x);
            Console.WriteLine(myStruct.x);
                
            //x += 1;

            //try {
                //Marshal.StructureToPtr(myStruct, ParentPtr, false);
            //}
            //finally {

            //}

        }
    }

    [DllImport("MyCSharpLibrary", EntryPoint = "Create")]
    public static extern IntPtr Create();

    [DllImport("MyCSharpLibrary", EntryPoint = "Destroy")]
    public static extern void Destroy(IntPtr componentPtr);

    public static String GetTypeName()
    {
        return "HEYLLO";
    }

    public Object Parent;
    public IntPtr ParentPtr;

    public Object GetParent() {
        return Parent;
    }

    public void SetParent(IntPtr parent) {
        Parent = new Object(parent);
        ParentPtr = parent;
    }
}

public static class NativeExports
{
    [UnmanagedCallersOnly(EntryPoint = "Create")]
    public static IntPtr Create()
    {
        Class1 component = new Class1();
        GCHandle handle = GCHandle.Alloc(component);
        return (IntPtr)handle;
    }

    [UnmanagedCallersOnly(EntryPoint = "Destroy")]
    public static void Destroy(IntPtr componentPtr)
    {
        GCHandle handle = (GCHandle)componentPtr;
        handle.Free();
    }

    [UnmanagedCallersOnly(EntryPoint = "Update")]
    public static void Update(IntPtr componentPtr)
    {
        var component = GCHandle.FromIntPtr(componentPtr).Target as Class1;
        component?.Update();
    }

    [UnmanagedCallersOnly(EntryPoint = "SetParent")]
    public static void SetParent(IntPtr componentPtr, IntPtr parentPtr)
    {
        var component = GCHandle.FromIntPtr(componentPtr).Target as Class1;
        component?.SetParent(parentPtr);
    }
}

