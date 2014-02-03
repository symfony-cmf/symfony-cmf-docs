Pull request and commit requirements
====================================

Pull requests
-------------

The pull request body should adhere to the Symfony standard `as defined here`_

Commits
-------

Before a pull request is merged the commit should be squashed (i.e. there
should be a single commit in the pull request with all of your work in it).

Squashing
~~~~~~~~~

You can easily squash a commit using `git rebase` as follows:

.. code-block:: bash

    $ git rebase --interactive HEAD~<number of commits in your pull request>

For example, if your pull request on github has 4 commits:

.. code-block:: bash

    $ git rebase --interactive HEAD~4

You will then be presented with a screen in your editor which looks something
like this:

.. code-block:: bash

    pick 608cc0f fix link to CoreBundle Documentation
    pick be141b0 Correctly use Testing features
    pick 6e2ebf5 Changed PHPunit config
    pick c396b08 Changed travis config

Tell github to pick the first one and squash the rest as follows:

.. code-block:: bash

    pick 608cc0f fix link to CoreBundle Documentation
    s be141b0 Correctly use Testing features
    s 6e2ebf5 Changed PHPunit config
    s c396b08 Changed travis config

Save the file and quit. GIT should now squash all your commits and ask you
for a new commit message.

Commit Message
~~~~~~~~~~~~~~

The commit message should be formatted as follows:

.. code-block::

    [<subject] <Short description>
    ==============================

    Fixes: <list of issues fixed>

    <long description>

    BC Breaks (as required)
    ---------

    <list of BC breaks and required migrations>

    Deprecations (as required)
    ------------

    <list of deprecations>

For example:

.. code-block::

    [Initializer] Initializers use ManagerRegistry
    ==============================================

    Fixes: #1234, #4321

    Initializers are now passed an instance of `ManagerRegistry` instead
    of the `Phpcr\Session`. This means that initializers can retrieve both
    the PHPCR session and the `DocumentManager`.

    This PR also introduces a requirement that all initializers provide a name
    which can be used in diagnostics.

    BC Breaks
    ---------

    - The `init` method of the InitializerInterface now accepts a
      `ManagerResistry` instead of a `PhpcrSession`. The PHPCR session can
      be retrieved using `$registry->getConnection` and the manager with
      `$registry->getManager()`;

    - The first argument to the `GenericInitializer` constructor is now the
      name of the initializer.

    Deprecations
    ------------

    This is just an example, there are no deprecations, but if there were
    deprecations they sure would be listed here.
