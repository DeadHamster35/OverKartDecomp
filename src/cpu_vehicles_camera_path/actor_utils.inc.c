s32 SearchSetHibasiraSub(s32 ActorIndex, s16 arg1) {
    s32 i;
    s32 a2 = 0;

    for (i = 0; i < NUM_PLAYERS; i++) {

        if (KameHibasira[i].UnkC == 0) {
            KameHibasira[i].UnkC = 1;
            KameHibasira[i].ActorIndex = ActorIndex;
            KameHibasira[i].unk10 = arg1;
            KameHibasira[i].unk14 = 0;
            a2 = 1;
            break;
        }
    }
    if (a2 == 0) {
        return -2;
    }
    return 0;
}

s32 KameHibasiraSet(s32 ActorIndex) {
    Object* actor = &SimpleObjectArray[ActorIndex];
    if (actor->Category != ACTOR_RED_SHELL) {
        return -1;
    }
    return SearchSetHibasiraSub(ActorIndex, 0);
}

s32 KameBlueHibasiraSet(s32 ActorIndex) {
    Object* actor = &SimpleObjectArray[ActorIndex];
    if (actor->Category != ACTOR_GREEN_SHELL) {
        return -1;
    }
    return SearchSetHibasiraSub(ActorIndex, 1);
}

s32 KameTogeHibasiraSet(s32 arg0) {
    Object* actor = &SimpleObjectArray[arg0];
    if (actor->Category != ACTOR_BLUE_SPINY_SHELL) {
        return -1;
    }
    return SearchSetHibasiraSub(arg0, 2);
}

void KameHibasiraClear(s32 ActorIndex) {
    struct unexpiredActors* phi;
    s32 i;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi = &KameHibasira[i];
        if (ActorIndex == phi->ActorIndex) {
            phi->UnkC = 0;
            phi->ActorIndex = 1000; // out of bounds
        }
    }
}

void KameHibasiraInit(void) {
    struct unexpiredActors* phi;
    s32 i;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi = &KameHibasira[i];
        phi->UnkC = 0;
        phi->ActorIndex = 1000; // out of bounds
    }
}

void KameHibasiraMove(void) {
    s32 someIndex;
    f32 var_f20;
    Object* temp_s1;
    struct unexpiredActors* var_s0;

    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        var_s0 = &KameHibasira[someIndex];
        if (var_s0->UnkC == 1) {
            temp_s1 = &SimpleObjectArray[var_s0->ActorIndex];
            var_s0->unk14++;
            switch (var_s0->unk10) {
                case 0:
                    if (var_s0->unk14 < 0xA) {
                        var_f20 = 0.3f;
                    } else {
                        var_f20 = 0.9f;
                    }
                    break;
                case 1:
                    if (var_s0->unk14 < 0xA) {
                        var_f20 = 0.15f;
                    } else {
                        var_f20 = 0.45f;
                    }
                    break;
                case 2:
                    if (var_s0->unk14 < 0xA) {
                        var_f20 = 0.15f;
                    } else {
                        var_f20 = 0.45f;
                    }
                    break;
                default:
                    var_f20 = 1.0f;
                    break;
            }
            if (!(var_s0->unk14 & 1)) {
                KwgetHibasira(temp_s1->Position, ((MakeRandomLimmit(30) + 20) * var_f20) / 50.0f, var_s0->unk10);
            }
        }
    }
}
