s32 search_set_hibasira_sub(s32 actorIndex, s16 arg1) {
    s32 i;
    s32 a2 = 0;

    for (i = 0; i < NUM_PLAYERS; i++) {

        if (kame_hibasira[i].unkC == 0) {
            kame_hibasira[i].unkC = 1;
            kame_hibasira[i].actorIndex = actorIndex;
            kame_hibasira[i].unk10 = arg1;
            kame_hibasira[i].unk14 = 0;
            a2 = 1;
            break;
        }
    }
    if (a2 == 0) {
        return -2;
    }
    return 0;
}

s32 kame_hibasira_set(s32 actorIndex) {
    Object* actor = &g_SimpleObjectArray[actorIndex];
    if (actor->category != ACTOR_RED_SHELL) {
        return -1;
    }
    return search_set_hibasira_sub(actorIndex, 0);
}

s32 kame_blue_hibasira_set(s32 actorIndex) {
    Object* actor = &g_SimpleObjectArray[actorIndex];
    if (actor->category != ACTOR_GREEN_SHELL) {
        return -1;
    }
    return search_set_hibasira_sub(actorIndex, 1);
}

s32 kame_toge_hibasira_set(s32 arg0) {
    Object* actor = &g_SimpleObjectArray[arg0];
    if (actor->category != ACTOR_BLUE_SPINY_SHELL) {
        return -1;
    }
    return search_set_hibasira_sub(arg0, 2);
}

void kame_hibasira_clear(s32 actorIndex) {
    struct unexpiredActors* phi;
    s32 i;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi = &kame_hibasira[i];
        if (actorIndex == phi->actorIndex) {
            phi->unkC = 0;
            phi->actorIndex = 1000; // out of bounds
        }
    }
}

void kame_hibasira_init(void) {
    struct unexpiredActors* phi;
    s32 i;

    for (i = 0; i < NUM_PLAYERS; i++) {
        phi = &kame_hibasira[i];
        phi->unkC = 0;
        phi->actorIndex = 1000; // out of bounds
    }
}

void kame_hibasira_move(void) {
    s32 someIndex;
    f32 var_f20;
    Object* temp_s1;
    struct unexpiredActors* var_s0;

    for (someIndex = 0; someIndex < NUM_PLAYERS; someIndex++) {
        var_s0 = &kame_hibasira[someIndex];
        if (var_s0->unkC == 1) {
            temp_s1 = &g_SimpleObjectArray[var_s0->actorIndex];
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
                kwget_hibasira(temp_s1->position, ((MakeRandomLimmit(30) + 20) * var_f20) / 50.0f, var_s0->unk10);
            }
        }
    }
}
