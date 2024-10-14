using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace VXLE;

[StructLayout(LayoutKind.Sequential)]
public struct MyStruct
{
    public float x;
    public float y;
    public float z;
}

public class Object
{
    [DllImport("/home/delstre-/cnt/prj/vxl/build/projects/VXLE/libs.so", EntryPoint = "GetTransform")]
    public static extern IntPtr GetTransform(IntPtr objectPtr);

    public IntPtr ObjectPtr;

    public IntPtr GetTransformInstance()
    {
        IntPtr transformPtr = GetTransform(ObjectPtr);
        return transformPtr;
    }

    [DllImport("/home/delstre-/cnt/prj/vxl/build/projects/VXLE/libs.so", EntryPoint = "GetPosition")]
    public static extern IntPtr GetPosition(IntPtr TransformPtr);

    public IntPtr GetPositionInstance(IntPtr TransformPtr)
    {
        return GetPosition(TransformPtr);
    }

    public Object(IntPtr objectPtr)
    {
        ObjectPtr = objectPtr;
    }
}

public class Component {
    public Object obj;
    public IntPtr pPosition;
    public MyStruct position;

    public Object Parent;
    public IntPtr ParentPtr;

    public void Start() {
        obj = GetParent();
        pPosition = obj.GetPositionInstance(obj.GetTransformInstance());
        position = Marshal.PtrToStructure<MyStruct>(pPosition);
    }

    public void PreUpdate() {}

    public virtual void Update() {}

    public void PostUpdate() {
        Marshal.StructureToPtr(position, pPosition, true);
    }

    public void UpdateComponent() {
        PreUpdate();
        Update();
        PostUpdate();
    }

    public Object GetParent() {
        return Parent;
    }

    public void SetParent(IntPtr parent) {
        Parent = new Object(parent);
        ParentPtr = parent;
    }
}

public class MyComponent : Component {
    public override void Update() {
        position.x = position.x + 0.01f;
    }
}

public static class NativeExports
{
    [UnmanagedCallersOnly(EntryPoint = "Create")]
    public static IntPtr Create()
    {
        MyComponent component = new MyComponent();
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
        var component = GCHandle.FromIntPtr(componentPtr).Target as MyComponent;
        component?.UpdateComponent();
    }

    [UnmanagedCallersOnly(EntryPoint = "Start")]
    public static void Start(IntPtr componentPtr)
    {
        var component = GCHandle.FromIntPtr(componentPtr).Target as MyComponent;
        component?.Start();
    }

    [UnmanagedCallersOnly(EntryPoint = "SetParent")]
    public static void SetParent(IntPtr componentPtr, IntPtr parentPtr)
    {
        var component = GCHandle.FromIntPtr(componentPtr).Target as MyComponent;
        component?.SetParent(parentPtr);
    }
}
