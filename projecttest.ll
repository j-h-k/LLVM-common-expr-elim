; ModuleID = 'TestModule'

define void @testfunction() {
L0:
  %t0 = alloca i32
  %t1 = alloca i32
  %t2 = alloca i32
  store i32 1, i32* %t0
  %t3 = load i32* %t0
  %t4 = add i32 %t3, 1
  store i32 %t4, i32* %t0
  %t5 = load i32* %t0
  %t6 = add i32 %t5, 1
  store i32 %t6, i32* %t0
  %t7 = load i32* %t0
  %t8 = add i32 %t7, 10
  store i32 %t8, i32* %t1
  %t9 = load i32* %t0
  %t10 = add i32 %t9, 10
  store i32 %t10, i32* %t1
  %t11 = load i32* %t0
  %t12 = add i32 %t11, 10
  store i32 %t12, i32* %t1
  %t13 = load i32* %t0
  %t14 = add i32 %t13, 10
  store i32 %t14, i32* %t1
  %t15 = load i32* %t1
  %t16 = add i32 %t15, 20
  store i32 %t16, i32* %t2
  %t17 = load i32* %t0
  %t18 = load i32* %t0
  %t19 = icmp eq i32 %t17, %t18
  br i1 %t19, label %L1, label %L2

L1:                                               ; preds = %L0
  %t20 = load i32* %t0
  %t21 = add i32 %t20, 1
  store i32 %t21, i32* %t1
  %t22 = load i32* %t0
  %t23 = add i32 %t22, 1
  store i32 %t23, i32* %t1
  %t24 = load i32* %t0
  %t25 = add i32 %t24, 1
  store i32 %t25, i32* %t1
  %t26 = load i32* %t0
  %t27 = add i32 %t26, 1
  store i32 %t27, i32* %t1
  %t28 = load i32* %t0
  %t29 = add i32 %t28, 1
  store i32 %t29, i32* %t1
  %t30 = load i32* %t0
  %t31 = add i32 %t30, 1
  store i32 %t31, i32* %t1
  %t32 = load i32* %t0
  %t33 = add i32 %t32, 1
  store i32 %t33, i32* %t1
  br label %L2

L2:                                               ; preds = %L1, %L0
  ret void
}
