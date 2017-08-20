#!/bin/bash

# Triggers a Travis job using the Travis API.
# Based on the trigger-travis script from plume-lib (https://github.com/mernst/plume-lib), provided under the MIT license:

# Copyright (c) 1998-2016 Michael Ernst
# Modifications (c) 2017 Sam Twidale (http://samcodes.co.uk/)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

# Get variables
USER=$1
REPO=$2
BRANCH=$3
TRAVIS_ACCESS_TOKEN=$4
MESSAGE=$5

# Login to Travis and get the token
travis login --skip-completion-check --github-token $TRAVIS_ACCESS_TOKEN
travis whoami --skip-completion-check
TOKEN=$(travis token --skip-completion-check)
IFS=' ' read -r -a array <<< "$TOKEN"
TOKEN=${array[${#array[@]}-1]}

# Check arguments and add message if necessary
if [ $# -eq 5 ] ; then
  MESSAGE=",\"message\": \"$5\""
elif [ -n "$TRAVIS_REPO_SLUG" ] ; then
  MESSAGE=",\"message\": \"Triggered by upstream build of $TRAVIS_REPO_SLUG with commit "`git rev-parse --short HEAD`"\""
fi

# For debugging purposes
#echo "USER=$USER"
#echo "REPO=$REPO"
#echo "BRANCH=$BRANCH"
#echo "TOKEN: ${array[${#array[@]}-1]}"
#echo "MESSAGE=$MESSAGE"

# Define the curl POST request content body
BODY="{
  \"request\": {
  \"branch\":\"$BRANCH\"
  $MESSAGE
}}"

# Make a POST request with curl
curl -s -X POST \
  -H "Content-Type: application/json" \
  -H "Accept: application/json" \
  -H "Travis-API-Version: 3" \
  -H "Authorization: token ${TOKEN}" \
  -d "$BODY" \
  https://api.travis-ci.org/repo/${USER}%2F${REPO}/requests | tee /tmp/travis-request-output.$$.txt

if grep -q '"@type": "error"' /tmp/travis-request-output.$$.txt; then
  cat /tmp/travis-request-output.$$.txt
  exit 1
elif grep -q 'access denied' /tmp/travis-request-output.$$.txt; then
  cat /tmp/travis-request-output.$$.txt
  exit 1
fi
