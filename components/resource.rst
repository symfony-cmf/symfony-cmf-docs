.. index::
    single: Resource; Components
    single: Resource

Resource
========

The Symfony CMF Resource component extends the `Puli`_ library to provide
a resource abstraction layer which allows you more flexibility in how
you access your data.

In particular this package provides `Puli`_ repository implementations for various
*database* backends. Repositories allow you to access your data via
paths, for example:

.. code-block:: php

    <?php
    $repo->get('/cmf/content/articles/my-article');

or if you want to locate / list multiple files:

.. code-block:: php

    <?php
    $repo->find('/cmf/content/articles/*');

or list all articles with node name ``my-article-1`` from all sites:

.. code-block:: php

    <?php
    $repo->find('/cmf/sites/*/content/articles/my-article');

Using this repository solves a number of problems in the domain of CMS
systems, it can be a key part solutions for:

- Implementing multiple site/portal systems.
- Combining different data sources into a single tree.
- Referencing resources by immutable names.

This documentation will not explain how to use `Puli`_ as it already has
some `very good documentation`_

Installation
------------

You can install this component `with composer`_ using the
`symfony-cmf/resource`_ package. If you are using the
``symfony-cmf/routing-bundle`` you do not need to specify the component
separately, it is required automatically.

Repositories
------------

The component aims to provide various Repository implementations. Currently
only `PHPCR`_ and `PHPCR-ODM`_ are supported.

PHPCR Repository
~~~~~~~~~~~~~~~~

The PHPCR repository acts as a proxy to the PHPCR Session.

It can be instantaited as follows:

.. code-block:: php

    use Symfony\Cmf\Component\Resource\Finder\Phpcr\TraversalFinder;

    $session = // phpcr session
    $parser = new SelectorParser();
    $finder = new TraversalFinder($session, $parser);
    $repository = new PhpcrRepository($session, new TraversalFinder());

    $resource = $repository->get('/foobar'); // returns ObjectResource
    $phpcrNode = $resource->getObject();

    $resourceCollection = $repository->find('/*/*bar/*');

    foreach ($resourceCollection as $resource) {
        echo $resource->getName();
        echo $resource->getPath();
        $phpcrNode = $resource->getObject();
    }

The roles of the classes are set out below:

- **parser**: This class is responsible for parsing a glob pattern and
  tokenizing it.
- **finder**: Class which can resolve a glob pattern to a set of results.
- **repository**: The Puli repository implementation

The `TraversalFinder` implements the
``Symfony\Cmf\Component\Resource\FinderInterface`` and traverses the PHPCR
nodes using the ``PHPCR\NodeInterface`` API. Currently this is the only
supported finder implementation for PHPCR.

.. _`with composer`: http://getcomposer.org
.. _`symfony-cmf/resource`: https://packagist.org/packages/symfony-cmf/resource
.. _`Puli`: https://github.com/puli/puli
.. _`PHPCR`: https://phpcr.github.io
.. _`PHPCR-ODM`: http://www.doctrine-project.org/projects/phpcr-odm.html
.. _`very good documentation`: http://puli.readthedocs.org/en/latest/
