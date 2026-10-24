
/* ---------- DAG-Based Common Subexpression Elimination ---------- */

static void optimize_tac(void) {
    for (int i = 0; i < code_count; i++) {
        if (strcmp(code[i].op, "+") != 0 &&
            strcmp(code[i].op, "*") != 0 &&
            strcmp(code[i].op, "-") != 0 &&
            strcmp(code[i].op, "/") != 0)
            continue;

        for (int j = 0; j < i; j++) {
            if (strcmp(code[j].op, code[i].op) != 0)
                continue;

            if (strcmp(code[j].op, "+") != 0 &&
                strcmp(code[j].op, "*") != 0 &&
                strcmp(code[j].op, "-") != 0 &&
                strcmp(code[j].op, "/") != 0)
                continue;

            int same_order =
                strcmp(code[i].arg1, code[j].arg1) == 0 &&
                strcmp(code[i].arg2, code[j].arg2) == 0;

            int reverse_order =
                (strcmp(code[i].op, "+") == 0 ||
                 strcmp(code[i].op, "*") == 0) &&
                strcmp(code[i].arg1, code[j].arg2) == 0 &&
                strcmp(code[i].arg2, code[j].arg1) == 0;

            if (!same_order && !reverse_order)
                continue;

            int valid = 1;

            for (int k = j + 1; k < i; k++) {
                if (strcmp(code[k].op, "goto") == 0 ||
                    strncmp(code[k].op, "if", 2) == 0) {
                    valid = 0;
                    break;
                }

                if (strcmp(code[k].op, "=") == 0 &&
                    (strcmp(code[k].result, code[j].arg1) == 0 ||
                     strcmp(code[k].result, code[j].arg2) == 0)) {
                    valid = 0;
                    break;
                }
            }

            if (!valid)
                continue;

            char old_temp[64];
            strcpy(old_temp, code[i].result);

            for (int k = i + 1; k < code_count; k++) {
                if (strcmp(code[k].arg1, old_temp) == 0)
                    strcpy(code[k].arg1, code[j].result);

                if (strcmp(code[k].arg2, old_temp) == 0)
                    strcpy(code[k].arg2, code[j].result);
            }

            code[i].op[0] = '\0';
            break;
        }
    }
}

/* ---------- TAC Output ---------- */

void print_tac(void) {
    optimize_tac();

    printf("\n=== THREE ADDRESS CODE ===\n\n");

    for (int i = 0; i < code_count; i++) {
        if (code[i].op[0] == '\0')
            continue;

        if (strcmp(code[i].op, "=") == 0) {
            printf("%d: %s = %s\n",
                   i, code[i].result, code[i].arg1);

        } else if (strcmp(code[i].op, "goto") == 0) {
            printf("%d: goto %s\n",
                   i, code[i].result);

        } else if (strncmp(code[i].op, "if", 2) == 0) {
            printf("%d: %s %s, %s goto %s\n",
                   i, code[i].op, code[i].arg1,
                   code[i].arg2, code[i].result);

        } else {
            printf("%d: %s = %s %s %s\n",
                   i, code[i].result, code[i].arg1,
                   code[i].op, code[i].arg2);
        }
    }
}
