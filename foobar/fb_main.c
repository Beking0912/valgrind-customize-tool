#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_debuginfo.h"

static Int executed_instructions_count = 0;

static void fb_post_clo_init(void)
{
}

static IRSB* fb_instrument(VgCallbackClosure* closure,
                           IRSB* bb, 
                           const VexGuestLayout* layout, 
                           const VexGuestExtents* vge,
                           const VexArchInfo* archinfo_host,
                           IRType gWordTy, IRType hWordTy )
{
    for (int i = 0; i < bb->stmts_used; i++) {
        IRStmt* st = bb->stmts[i];
        if (!st || st->tag == Ist_NoOp) continue;
      
        if (st->tag == Ist_IMark) {
            executed_instructions_count++;
        }
    }
    return bb;
}

static void fb_fini(Int exitcode)
{
    VG_(printf)("Total number of executed instruction: %d\n", executed_instructions_count);
}

static void fb_pre_clo_init(void)
{
    VG_(details_name)            ("foobar");
    VG_(details_version)         (NULL);
    VG_(details_description)     ("counts executed instructions");
    VG_(details_copyright_author) ("Copyright (C) 2023 by Biqing Su.");
    VG_(details_bug_reports_to) (VG_BUGS_TO);

    VG_(details_avg_translation_sizeB) (275);

    VG_(basic_tool_funcs)
    (fb_post_clo_init,
     fb_instrument,
     fb_fini);

    /* No needs, no core events to track */
}

VG_DETERMINE_INTERFACE_VERSION(fb_pre_clo_init)