/*
 * Copyright (c) Facebook, Inc. and its affiliates.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <proxygen/lib/http/ProxyStatus.h>

#include <glog/logging.h>

using proxygen::StructuredHeaders::StructuredHeaderItem;

namespace {

constexpr folly::StringPiece kProxyParam{"e_proxy"};
constexpr folly::StringPiece kUpstreamIPParam{"e_upip"};

} // namespace

namespace proxygen {

ProxyStatus::ProxyStatus(StatusType statusType) : statusType_{statusType} {
  pIdent_.identifier = getStatusTypeString(statusType_);
}

StatusType ProxyStatus::getStatusType() const {
  return statusType_;
}

void ProxyStatus::setStatusType(StatusType statusType)
{
  statusType_ = statusType;
  pIdent_.identifier = getStatusTypeString(statusType_);
};

ProxyStatus& ProxyStatus::setProxyStatusParameter(folly::StringPiece name,
                                                  const std::string& text) {
  if (text.empty()) {
    return *this;
  }

  pIdent_.parameterMap[name.str()] =
      StructuredHeaderItem(StructuredHeaderItem::Type::STRING, text);
  return *this;
}

ProxyStatus& ProxyStatus::setProxy(const std::string& proxy) {
  return setProxyStatusParameter(kProxyParam, proxy);
}

ProxyStatus& ProxyStatus::setUpstreamIP(const std::string& upstreamIP) {
  return setProxyStatusParameter(kUpstreamIPParam, upstreamIP);
}

bool ProxyStatus::hasUpstreamIP() const {
  return pIdent_.parameterMap.find(std::string(kUpstreamIPParam)) !=
         pIdent_.parameterMap.end();
}

std::string ProxyStatus::str() const {
  StructuredHeaders::ParameterisedList plist;
  StructuredHeadersEncoder encoder;
  plist.emplace_back(std::move(pIdent_));
  encoder.encodeParameterisedList(plist);

  return encoder.get();
}

} // namespace proxygen
