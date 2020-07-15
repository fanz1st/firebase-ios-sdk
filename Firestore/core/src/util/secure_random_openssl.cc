/*
 * Copyright 2018 Google
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Firestore/core/src/util/secure_random.h"

#include "Firestore/core/src/util/config.h"

#ifndef HAVE_OPENSSL_RAND_H
#error HAVE_OPENSSL_RAND_H must be set to 1 or 0; verify #include for config.h.
#elif HAVE_OPENSSL_RAND_H

#include "openssl/err.h"
#include "openssl/rand.h"

namespace firebase {
namespace firestore {
namespace util {

SecureRandom::result_type SecureRandom::operator()() {
  result_type result;
  int rc = RAND_bytes(reinterpret_cast<uint8_t*>(&result), sizeof(result));
  if (rc <= 0) {
    // OpenSSL's RAND_bytes can fail if there's not enough entropy. BoringSSL
    // won't fail this way.
    ERR_print_errors_fp(stderr);
    abort();
  }
  return result;
}

}  // namespace util
}  // namespace firestore
}  // namespace firebase

#endif  // HAVE_OPENSSL_RAND_H
