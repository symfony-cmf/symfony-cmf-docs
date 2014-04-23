Commit Conventions
==================

The Symfony CMF uses a convention for the commits. This isn't a requirement
when contributing to the CMF, but it is created to have meaningfull commits
for big new features.

Squashing
---------

Before a Pull Request is merged, the commits should be squashed (i.e. there
should be a single commit in the Pull Request with all your work in it).

You can easily squash a commit using `git rebase` as follows:

.. code-block:: bash

    $ git rebase --interactive HEAD~<number of commits in your pull request>

For example, if your pull request on github has 4 commits:

.. code-block:: bash

    $ git rebase --interactive HEAD~4

You will then be presented with a screen in your editor which looks something
like this:

.. code-block:: bash

    pick 5d4530b port features from simple cms into routing bundle to simplify things
    pick 1a0eea3 cs fixes and cleanups according to feedback
    pick 8cbab56 convert settings to options
    pick 8f3e4f9 cleanups for the options refactoring

Tell github to pick the first one and squash the rest as follows:

.. code-block:: bash

    pick 5d4530b port features from simple cms into routing bundle to simplify things
    s 1a0eea3 cs fixes and cleanups according to feedback
    s 8cbab56 convert settings to options
    s 8f3e4f9 cleanups for the options refactoring

Save the file and quit. GIT should now squash all your commits and ask you
for a new commit message.

Commit Message
--------------

The commit message should be formatted as follows:

.. code-block:: text

    [<scope>] <short description>

    Fixes: <list of issues fixed>

    <long description>

    BC Breaks (as required)
    ---------

    <list of BC breaks and required migrations>

    Deprecations (as required)
    ------------

    <list of deprecations>

For example, the commit message of a PR fixing 2 issues and adding 2 BC breaks
would be:

.. code-block:: text

    [Initializer] Initializers use ManagerRegistry

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

Short Commit Message
~~~~~~~~~~~~~~~~~~~~

Not all Pull Requests require this much information for the commit. In most
cases, a more simpler commit convention is enough:

.. code-block:: text

    <bug|feature|minor> [<scope>] <short description>

Where ``bug`` refers to a commit fixing bugs, ``feature`` to a commit adding
features and ``minor`` to commits adding less relevant things (fixing code
standard, adding comments, fixing typos, etc.).
