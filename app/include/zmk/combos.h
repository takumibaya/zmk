/*
 * Copyright (c) 2020 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <zephyr/devicetree.h>

#define ZMK_COMBOS_UTIL_ONE(n) +1

#define ZMK_COMBOS_DT_LEN                                                                          \
    COND_CODE_1(DT_HAS_COMPAT_STATUS_OKAY(zmk_combos),                                             \
                (0 DT_FOREACH_CHILD_STATUS_OKAY(DT_INST(0, zmk_combos), ZMK_COMBOS_UTIL_ONE)),     \
                (0))

#if IS_ENABLED(CONFIG_ZMK_COMBOS_RUNTIME)
#include <zmk/behavior.h>

#define ZMK_COMBOS_LEN MAX(ZMK_COMBOS_DT_LEN, CONFIG_ZMK_COMBOS_RUNTIME_SLOTS)

struct zmk_combo_config {
    uint16_t key_positions[CONFIG_ZMK_COMBOS_RUNTIME_MAX_KEYS];
    uint8_t key_position_len; /* Zero means inactive. */
    int16_t require_prior_idle_ms;
    int32_t timeout_ms;
    uint32_t layer_mask; /* Bits are layer IDs, not display indices. */
    struct zmk_behavior_binding behavior;
    bool slow_release;
};

size_t zmk_combo_slot_count(void);
size_t zmk_combo_dt_count(void);

/* Copy the slot or DT default; slots without a DT default return an empty config. */
int zmk_combo_read(size_t slot, struct zmk_combo_config *out);
int zmk_combo_read_default(size_t slot, struct zmk_combo_config *out);

/*
 * Set or clear one slot (NULL clears); apply replaces the entire slot table in one work item.
 * Behavior names must have static lifetime. Calls are synchronous, from thread context only.
 * Both return -EBUSY while a combo is pending or held, and -ERANGE for invalid bounds.
 */
int zmk_combo_set(size_t slot, const struct zmk_combo_config *config);
int zmk_combo_apply(const struct zmk_combo_config configs[ZMK_COMBOS_LEN]);
#else
#define ZMK_COMBOS_LEN ZMK_COMBOS_DT_LEN
#endif
