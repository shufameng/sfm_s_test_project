function Trig_InitMap_Actions takes nothing returns nothing
    local integer i = 0
    local player p = null

    // 初始化全局变量
    call TriggerExecute( gg_trg_InitGlobalVariables )

    // 创建选取英雄的视野,创建选取英雄的提示漂浮文字
    call CreateFogModifierRadiusLocBJ(true, udg_p_goodLeader, FOG_OF_WAR_VISIBLE, udg_loc_pickHero, 900.0)

    
    loop
        exitwhen (i > 23)
        set p = Player(i)
        call SetPlayerFlagBJ(PLAYER_STATE_GIVES_BOUNTY, true, p)
        set i = i + 1
    endloop

    set p = null
endfunction

//===========================================================================
function InitTrig_InitMap takes nothing returns nothing
    set gg_trg_InitMap = CreateTrigger(  )
    call TriggerAddAction( gg_trg_InitMap, function Trig_InitMap_Actions )
endfunction