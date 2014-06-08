# give me a nice name
git config --global user.name "Sf Travis Bot"
git config --global user.email "sf-travis-bot@travis-ci.org"

# clone the demo repository
git clone https://${GH_TOKEN}@github.com/WouterJ/symfony-cmf-docs-demo

# create a new directory name
if [[ "false" == ${TRAVIS_PULL_REQUEST} ]]; then
    DEMO_PREFIX=${TRAVIS_BRANCH}
else
    DEMO_PREFIX=${TRAVIS_PULL_REQUEST}
fi

DEMO_DIR_NAME=symfony-cmf-docs-demo/${DEMO_PREFIX}-${TRAVIS_BUILD_NUMBER}

# remove all previous demos for this PR
rm -rf symfony-cmf-docs-demo/${DEMO_PREFIX}-*

mkdir $DEMO_DIR_NAME

# copy the build to the demo dir
cp -rf _build/html/* $DEMO_DIR_NAME

cd symfony-cmf-docs-demo

# commit all removed and added files
git add -A
git commit -m "Travis build ${TRAVIS_BUILD_NUMBER}"

# push to origin
git push origin

COMMIT=git rev-parse HEAD

# update status of PR
curl "https://api.github.com/repos/WouterJ/symfony-cmf-docs/statuses/${COMMIT}?access_token=${GH_TOKEN}" \
    -H "Content-Type: application/json" \
    -X POST \
    -d "{\"state\": \"success\", \"description\": \"Build succeeded!\", \"target_url\": \"http://wouterj.github.com/${DEMO_DIR_NAME}\"}"
