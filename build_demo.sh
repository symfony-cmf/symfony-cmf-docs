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

if [ $? != 0 ]; then
    echo "Error while pushing demo to GitHub"
else
    echo "Pushed demo to github, visit your demo at http://wouterj.github.com/symfony-cmf-docs-demo/${DEMO_DIR_NAME}"
fi
