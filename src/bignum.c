/*
    This file is part of sscg.

    sscg is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    sscg is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with sscg.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2017 by Stephen Gallagher <sgallagh@redhat.com>
*/

#include "include/sscg.h"
#include "include/bignum.h"

static int
_sscg_bignum_destructor(TALLOC_CTX *mem_ctx)
{
    struct sscg_bignum *bn =
        talloc_get_type_abort(mem_ctx, struct sscg_bignum);

    BN_free(bn->bn);

    return 0;
}

int
sscg_init_bignum(TALLOC_CTX *mem_ctx, unsigned long num,
                 struct sscg_bignum **bn)
{
    int ret = EOK;
    struct sscg_bignum *bignum;

    TALLOC_CTX *tmp_ctx = talloc_new(NULL);
    if (!tmp_ctx) {
        return ENOMEM;
    }

    bignum = talloc_zero(tmp_ctx, struct sscg_bignum);
    if (!bignum) {
        ret = ENOMEM;
        goto done;
    }

    BIGNUM *sslbn = BN_new();
    if (!sslbn) {
        ret = ENOMEM;
        goto done;
    }

    bignum->bn = sslbn;
    talloc_set_destructor((TALLOC_CTX *)bignum, _sscg_bignum_destructor);

    BN_set_word(bignum->bn, num);

    ret = EOK;

done:
    if (ret == EOK) {
        *bn = talloc_steal(mem_ctx, bignum);
    }
    talloc_zfree(tmp_ctx);

    return ret;
}