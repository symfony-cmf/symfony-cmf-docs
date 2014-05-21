git config --global user.name "Sf Travis Bot"
git config --global user.email "sf-travis-bot@travis-ci.org"

git clone https://${GH_TOKEN}@github.com/WouterJ/symfony-cmf-docs-demo

if [[ ${TRAVIS_PULL_REQUEST} ]]
    DEMO_DIR_NAME=symfony-cmf-docs-demo/${TRAVIS_PULL_REQUEST}-${TRAVIS_BUILD_NUMBER}
else
    DEMO_DIR_NAME=symfony-cmf-docs-demo/${TRAVIS_REPO_SLUG//\//-}-${TRAVIS_BRANCH}-${TRAVIS_BUILD_NUMBER}
fi

DEMO_DIR_NAME=symfony-cmf-docs-demo/${TRAVIS_PULL_REQUEST}-${TRAVIS_BUILD_NUMBER}

mkdir $DEMO_DIR_NAME

cp -rf _build/html/* $DEMO_DIR_NAME

cd symfony-cmf-docs-demo

git add .

git commit -m "Travis build ${TRAVIS_BUILD_NUMBER}"
git push origin

if [ $? != 0 ]; then
    echo "Error while pushing demo to GitHub"
else
    echo "Pushed demo to github, visit your demo at http://wouterj.github.com/symfony-cmf-docs-demo/${DEMO_DIR_NAME}"
fi
