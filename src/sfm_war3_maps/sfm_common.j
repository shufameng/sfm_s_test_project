// 为用户增加金币,有特效
function SFM_AddGoldToPlayer takes player p, integer sum, location specialEffectLoc, sound sd returns nothing
    local texttag tt = null
    local effect e = null
    local location loc = null
    call AdjustPlayerStateBJ(sum, p, PLAYER_STATE_RESOURCE_GOLD)
    if (specialEffectLoc == null) then
        set loc = GetCameraTargetPositionLoc()
    else
        set loc = specialEffectLoc
    endif
    if (GetLocalPlayer() == p) then
        set tt = CreateTextTagLocBJ("+" + I2S(sum), loc, 0, 10, 100, 100, 0, 0)
        call SetTextTagPermanentBJ(tt, false)
        call SetTextTagVelocityBJ(tt, 80, 90)
        call SetTextTagLifespanBJ(tt, 1.5)
        if (sd != null) then
            call PlaySoundAtPointBJ(sd, 100, loc, 0)
        endif
    endif
    call RemoveLocation(loc)
    set loc = null
    set tt = null
    set e = null
endfunction

// 从用户输入的字符串中获取命令
function SFM_GetCmdFromString takes string src, string cmdPrefix returns string
    local string ret = ""
    local string subStr = ""
    local integer srcLen = StringLength(src)
    local integer i = 0
    if (SubStringBJ(src, 1, StringLength(cmdPrefix)) != cmdPrefix) then
        return ret
    endif
    set i = StringLength(cmdPrefix) + 1
    loop
        exitwhen (i > srcLen)
        set subStr = SubStringBJ(src, i, i)
        if (subStr == " ") then
            return ret
        endif
        set ret = ret + subStr
        set i = i + 1
    endloop
    return ret
endfunction

// 从用户输入的字符串中获取命令参数
function SFM_GetCmdParamFromString takes string src, string cmdPrefix, string cmd returns string
    return SubStringBJ(src, StringLength(cmdPrefix) + StringLength(cmd) + 2, StringLength(src))
endfunction

// 根据用户输入的字符串,执行测试命令
function SFM_ExecuteTestCmd takes string src returns nothing
    local string cmdPrefix = "-"
    local string cmd = SFM_GetCmdFromString(src, cmdPrefix)
    local string cmdParam = ""
    local integer cmdParamInt = 0
    local group grp = CreateGroup()
    local unit u = null

    call GroupEnumUnitsSelected(grp, GetTriggerPlayer(), null)
    if (cmd == "kill") then
        loop
            exitwhen (IsUnitGroupEmptyBJ(grp))
            set u = FirstOfGroup(grp)
            call GroupRemoveUnit(grp, u)
            call KillUnit(u)
        endloop

    elseif (cmd == "copy") then
        set cmdParam = SFM_GetCmdParamFromString(src, cmdPrefix, cmd)
        set cmdParamInt = S2I(cmdParam)
        if (cmdParamInt > 10) then
            set cmdParamInt = 10
        endif      
        loop
            exitwhen (IsUnitGroupEmptyBJ(grp))
            set u = FirstOfGroup(grp)
            call GroupRemoveUnit(grp, u)
            call CreateNUnitsAtLoc(cmdParamInt, GetUnitTypeId(u), GetTriggerPlayer(), GetUnitLoc(u), GetUnitFacing(u))
        endloop 

    elseif (cmd == "setlevel" or cmd == "setlv") then
        set cmdParam = SFM_GetCmdParamFromString(src, cmdPrefix, cmd)
        set cmdParamInt = S2I(cmdParam)
        loop
            exitwhen (IsUnitGroupEmptyBJ(grp))
            set u = FirstOfGroup(grp)
            call GroupRemoveUnit(grp, u)
            call SetHeroLevel(u, cmdParamInt, true)
        endloop

    elseif (cmd == "setowner") then
        set cmdParam = SFM_GetCmdParamFromString(src, cmdPrefix, cmd)
        set cmdParamInt = S2I(cmdParam)
        if (Player(cmdParamInt) != null) then
            loop
                exitwhen (IsUnitGroupEmptyBJ(grp))
                set u = FirstOfGroup(grp)
                call GroupRemoveUnit(grp, u)
                call SetUnitOwner(u, Player(cmdParamInt), true)
            endloop
        endif

    elseif (cmd == "settimeofday") then
        set cmdParam = SFM_GetCmdParamFromString(src, cmdPrefix, cmd)
        set cmdParamInt = S2I(cmdParam)
        call SetTimeOfDay(cmdParamInt)

    elseif (cmd == "addgold") then
        set cmdParam = SFM_GetCmdParamFromString(src, cmdPrefix, cmd)
        set cmdParamInt = S2I(cmdParam)
        call AdjustPlayerStateBJ(cmdParamInt, GetTriggerPlayer(), PLAYER_STATE_RESOURCE_GOLD)

    elseif (cmd == "resetcd") then
        loop
            exitwhen (IsUnitGroupEmptyBJ(grp))
            set u = FirstOfGroup(grp)
            call GroupRemoveUnit(grp, u)
            call UnitResetCooldown(u)
        endloop

    elseif (cmd == "playanimbyname" or cmd == "animbyname") then
        set cmdParam = SFM_GetCmdParamFromString(src, cmdPrefix, cmd)
        loop
            exitwhen (IsUnitGroupEmptyBJ(grp))
            set u = FirstOfGroup(grp)
            call GroupRemoveUnit(grp, u)
            call SetUnitAnimation(u, cmdParam)
        endloop

    elseif (cmd == "help") then
        call DisplayTimedTextToPlayer(GetTriggerPlayer(), 0, 0, 30, "-kill,-copy,-setlevel/setlv,-setowner,-settimeofday,-addgold,-resetcd,-playanimbyname/animbyname,help")

    endif

    call DestroyGroup(grp)
    set grp = null
    set u = null
endfunction
