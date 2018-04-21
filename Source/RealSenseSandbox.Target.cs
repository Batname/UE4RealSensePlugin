// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class RealSenseSandboxTarget : TargetRules
{
	public RealSenseSandboxTarget(TargetInfo Target) : base(Target)
    {
		Type = TargetType.Game;
        ExtraModuleNames.Add("RealSenseSandbox");
    }
}
