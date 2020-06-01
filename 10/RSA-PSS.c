/*************************************************************************
 > File Name: RSA-PSS.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Thu 21 May 2020 02:51:48 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <gmp.h>
#include "ari.h"
#include "sha256.h"
#include "tools.h"
#define hLen 32
#define sLen hLen

typedef unsigned char uint8;

int EMckecking(uint8 *M,long long int mLen,uint8 *EM,long long int emBits);
int ckecking(uint8 *M,long long int mLen,uint8 *S,long long int k,mpz_t n,mpz_t e);
uint8 *sign(uint8 *M,long long int mLen,mpz_t n,mpz_t d);
uint8 *EMcode(uint8 *M,long long int mLen,long long int emBits);
uint8 *MGF1(uint8 *mgfseed, long long int maskLen);
uint8 *getsalt(long long int l);

int main(){
    unsigned char M[] = {10,11,12,15,18,19,30,31,32,40,42,58,66,79,88,91,96,97,101,150,130,200,199,166,201};
    long long int mLen = strlen(M);
    mpz_t n,d,e;
    mpz_inits(n,e,d,NULL);
    mpz_set_str(e , "65537",10);

    mpz_set_str(n, "21378032245774894186324720788457768851857953294637267751313371903474996018902810092972224806315945430514626988743400353365786674788848003569698586194388463460699531620948408197942261177369324808332585418351947368544183614904162658914539989430070735676083960582943505227316151479174351490943931346982185481068889458087344890907035731467000101100009111593455801160870652558847164438348031498067369123608755518383163346962891967964682970251625764813457371461595048927486942272152822570449609158417324070867001419543608370026546341531367233199832189762919523227554947408242727690461831545997600374969434706782376320559561",10);
    mpz_set_str(d , "13085102850405329895940153649208766646679432053055210927814587187939575969334380946175257120108607885616731724467899991987963542311668962802613624160423864736904359544115910805381019345850330276964971412664144174157825068713331109139842487999112829255389047329358923488846912437792391102853729375052922599258215311601018992134762683570752403675370812499995354701024990414541327012769030147878934713424171374951602988478984432403148854042370903764361797455965930292322795814453835335323397068237664481359506461188857661605832041501219728374514303209642746672993156029099655958158717907546664548938973389857200804582177",10);
    long long int k = 256;

    uint8 *S = sign(M, mLen, n, d);
    int res = ckecking(M,mLen,S,k,n, e);
    printf("%d\n",res);
    S[0]=128;
    res = ckecking(M,mLen,S,k,n, e);
    printf("%d\n",res);
    mpz_clears(n,e,d,NULL);
    return 0;
}


int EMckecking(uint8 *M,long long int mLen,uint8 *EM,long long int emBits){
    long long int emLen = (emBits%8 == 0)?(emBits/8):(emBits/8+1);
    uint8 padd1[8];
    memset(padd1,0,8*sizeof(uint8));

    SHA256_CTX ctx;
    uint8 hash[hLen];
    memset(hash,0,hLen * sizeof(uint8));
    sha256_init(&ctx);
    sha256_update(&ctx, M, mLen );
    sha256_final(&ctx, hash);

    if(emLen < hLen + sLen + 2){
        printf("emLen is wrong!\n");
        return 0;
    }

    if(EM[emLen-1]!=0xbc){
        printf("%x\n",EM[emLen-1] );
        printf("bc wrong!\n");
        return 0;
    }

    uint8 maskedDB[emLen - hLen - 1],H[hLen];
    for(int i =0;i<emLen-hLen -1;i++)
        maskedDB[i] = EM[i];
    for(int i = 0 ; i<hLen ;i++)
        H[i] = EM[i + emLen - hLen - 1];
    uint8 tmp = (~((1 << (8 - 8 * emLen + emBits)) - 1)) & 0xff;
    if ((maskedDB[0] & tmp) != 0)
    {
        printf("maskedDB wrong!\n");
        return 0;
    }

    uint8 *dbMask = MGF1(H,emLen-hLen - 1);
    uint8 DB[emLen - hLen - 1];
    for(int i = 0; i< emLen - hLen -1;i++)
        DB[i] = maskedDB[i]^dbMask[i];
    tmp = (1 << (8 - 8 * emLen + emBits)) - 1;
    DB[0] = DB[0] & tmp;
    for (int i = 0; i < emLen - hLen - sLen - 2; i++)
        if (DB[i] != 0)
        {
            printf("DB wrong!\n");
            return 0;
        }
    if (DB[emLen - hLen - sLen - 2] != 0x01)
    {
        printf("DB wrong!\n");
        return 0;
    }

    uint8 salt[sLen];
    for (int i = 0; i < sLen; i++)
        salt[i] = DB[emLen - hLen - sLen - 1 + i];

    uint8* M_ = array_append(padd1, 8, hash, hLen);
    M_ = array_append(M_, hLen + 8, salt, sLen);
    memset(hash,0,hLen * sizeof(uint8));
    sha256_init(&ctx);
    sha256_update(&ctx, M_, mLen );
    sha256_final(&ctx, hash);
    int flag = 1;
    for(int i =0;i<hLen;i++){
        if(hash[i]!=H[i]){
            flag = 0;
            break;
        }
    }

    if(flag)
        return 1;
    printf("Hash wrong!\n");
    return 0;
}

uint8 *sign(uint8 *M,long long int mLen,mpz_t n,mpz_t d){
    long long int emBits = mpz_sizeinbase(n,2)-1;
    long long int emLen = (emBits%8 == 0)?(emBits/8):(emBits/8+1);
    uint8 *EM = EMcode(M, mLen, emBits);
    mpz_t m,s;
    mpz_inits(m,s,NULL);
    OS2IP(m, EM, emLen);
    fast_pow_mod(s, m, d, n);

    long long int k = mpz_sizeinbase(n,2)/8;
    printf("k = %d\n",k );
    uint8 *S = I2OSP(s, k);
    mpz_clears(m,s,NULL);
    printf("sign over!\n\n");
    return S;
}

int ckecking(uint8 *M,long long int mLen,uint8 *S,long long int k,mpz_t n,mpz_t e){
    mpz_t s,m;
    long long int emBits = mpz_sizeinbase(n,2)-1;
    long long int emLen = (emBits%8 == 0)?(emBits/8):(emBits/8+1);
    mpz_inits(s,m,NULL);
    OS2IP(s, S, k);
    fast_pow_mod(m, s, e, n);
    uint8 *EM = I2OSP(m, emLen);
    int res=EMckecking(M, mLen, EM, emBits);
    mpz_clears(s,m,NULL);
    return res;
}

uint8 *EMcode(uint8 *M,long long int mLen,long long int emBits){
    long long int emLen = (emBits%8 == 0)?(emBits/8):(emBits/8+1);
    uint8 padd1[8],padd2[emLen - sLen - hLen - 1];
    memset(padd1,0,8*sizeof(uint8));
    memset(padd2,0,(emLen - sLen - hLen - 1)*sizeof(uint8));
    padd2[emLen - sLen - hLen - 2] = 1;
    uint8 *salt = (uint8 *)malloc(sizeof(uint8) * sLen);

    SHA256_CTX ctx;
    uint8 hash[hLen];
    memset(hash,0,hLen * sizeof(uint8));
    sha256_init(&ctx);
    sha256_update(&ctx, M, mLen );
    sha256_final(&ctx, hash);

    salt = getsalt(sLen);
    uint8 *M_ = array_append(padd1, 8, hash, hLen);
    M_ = array_append(M_, 8+hLen, salt, sLen);

    memset(hash,0,hLen * sizeof(uint8));
    sha256_init(&ctx);
    sha256_update(&ctx, M_, mLen );
    sha256_final(&ctx, hash);

    uint8 *DB = array_append(padd2, emLen - sLen - hLen - 1, salt, sLen);
    uint8 *dbMask = MGF1(hash, emLen - hLen - 1);

    uint8 *maskedDB = (uint8 *)malloc(sizeof(uint8) * (emLen - hLen - 1));
    for(int i = 0; i< emLen - hLen -1;i++)
        maskedDB[i] = DB[i]^dbMask[i];

    int tmp = (1 << (8 - 8 * emLen + emBits)) - 1;
    maskedDB[0] = maskedDB[0] & tmp;

    uint8 *EM = (uint8 *)malloc(sizeof(uint8) * (emLen));
    uint8 *EM_tmp = array_append(maskedDB, emLen - hLen - 1, hash, hLen);
    for (int i = 0; i < emLen - 1; i++)
        EM[i] = EM_tmp[i];
    EM[emLen - 1] = 0xbc;
    if(salt!=NULL)
        free(salt);
    if(maskedDB!=NULL)
        free(maskedDB);
    return EM;
}

uint8 *MGF1(uint8 *mgfseed, long long int maskLen){
    mpz_t tmp;
    mpz_init_set_ui(tmp, hLen);
    mpz_mul_2exp(tmp, tmp, 32);
    if (mpz_cmp_ui(tmp, maskLen) < 0){
        printf("MGF1: maskLen overflow");
        exit(-1);
    }
    uint8 *T = (uint8 *)malloc(sizeof(uint8) * maskLen);
    uint8 *C, hash[hLen];
    uint8 *x_c;
    SHA256_CTX ctx;
    int counter = 0, T_index = 0, i = 0, isquit = 0;
    long long int k = (maskLen % hLen == 0)?(maskLen/hLen - 1):(maskLen / hLen);
    for (counter = 0; counter < k; counter++)
    {
        mpz_set_ui(tmp, counter);
        memset(hash, 0, sizeof(uint8) * hLen);
        C = I2OSP(tmp, 4);
        x_c = array_append(mgfseed, sLen, C, 4);
        sha256_init(&ctx);
        sha256_update(&ctx, x_c, sLen + 4);
        sha256_final(&ctx, hash);
        for (i = 0; i < hLen; i++)
        {
            if (T_index == maskLen)
            {
                isquit = 1;
                break;
            }
            T[T_index++] = hash[i];
        }
        if (isquit == 1)
            break;
    }
    if (x_c != NULL)
        free(x_c);
    if (C != NULL)
        free(C);

    mpz_clear(tmp);
    return T;

}

uint8 *getsalt(long long int l){
    uint8 *salt = (uint8 *)malloc(sizeof(uint8) * l);
    srand(time(NULL));
    for (int i = 0; i < l; i++){
        salt[i] = rand() % 256;
    }
    return salt;
}
