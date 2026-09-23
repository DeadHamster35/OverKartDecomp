/*
These are default functions that overwrite existing game Code calls.
If the defined functions have not been made in an external codebase, it will use the default.
*/

.ifndef GameCode
.definelabel GameCode, GameCodeDefault
.endif

.ifndef TitleMenu
.definelabel TitleMenu, TitleMenuDefault
.endif

.ifndef Title2
.definelabel Title2, title2Default
.endif

.ifndef MapStartup
.definelabel MapStartup, MapStartupDefault
.endif

.ifndef DisplayObject
.definelabel DisplayObject, DisplayObjectDefault
.endif

.ifndef CollideObject
.definelabel CollideObject, CollideObjectDefault
.endif

.ifndef ItemboxCollideCheck
.definelabel ItemboxCollideCheck, ItemboxCollideCheckDefault
.endif

.ifndef RAMCheck
.definelabel RAMCheck, RAMCheckDefault
.endif

.ifndef RAMCheckEnd
.definelabel RAMCheckEnd, RAMCheckEndDefault
.endif

.ifndef PrintMenuFunction
.definelabel PrintMenuFunction, PrintMenuFunctionDefault
.endif


.ifndef DrawPerScreen
.definelabel DrawPerScreen, DrawPerScreenDefault
.endif


.ifndef AllRun
.definelabel AllRun, AllRunDefault
.endif


.ifndef CheckHit
.definelabel CheckHit, CheckHitDefault
.endif


.ifndef ExecuteItemHook
.definelabel ExecuteItemHook, ExecuteItemHookDefault
.endif


.ifndef MiniMapDraw
.definelabel MiniMapDraw, MiniMapDrawDefault
.endif

.ifndef KWKumo_Alloc_Hook
.definelabel KWKumo_Alloc_Hook, KWKumoAllocHookDefault
.endif

.ifndef KWChart_Kumo_Hook
.definelabel KWChart_Kumo_Hook, KWChartKumoHookDefault
.endif

.ifndef BalloonCheck
.definelabel BalloonCheck, BalloonCheckDefault
.endif
