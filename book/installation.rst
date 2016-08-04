.. index::
    single: Installation; Getting Started
    single: Standard Edition

Installing the Standard Edition
===============================

The Symfony CMF Standard Edition (SE) is a distribution based on all the
main components needed for most common use cases.

The goal of this tutorial is to install the CMF bundles, with the minimum
necessary configuration and some very simple examples, into a working Symfony2
application.

After that, you get a quick introduction of the bundles you have installed.
This can be used to familiarize yourself with the CMF or as a starting point
for a new custom application.

This tutorial is aimed at people who want to get started with a project based
on the Symfony CMF. There are two alternate articles in the cookbook you might
want to look at, depending on what you need:

* :doc:`../cookbook/editions/cmf_core` - a guide for adding the CMF to a standard
  Symfony project.
* :doc:`../cookbook/editions/cmf_sandbox` for instructions on how to install a
  demonstration sandbox. The sandbox contains many examples of what you can do
  with the CMF.

Preconditions
-------------

As Symfony CMF is based on Symfony2, you should make sure you meet the
`Requirements for running Symfony2`_. Additionally, you need to have `SQLite`_
PDO extension (``pdo_sqlite``) installed, since it is used as the default
storage medium.

.. note::

    By default, Symfony CMF uses Jackalope + Doctrine DBAL and SQLite as the
    underlying DB. However, Symfony CMF is storage agnostic, which means you
    can use one of several available data storage mechanisms without having to
    rewrite your code. For more information on the different available
    mechanisms and how to install and configure them, refer to
    :doc:`../bundles/phpcr_odm/introduction`.

Installation
------------

You can install the Standard Edition in 2 ways:

1) Composer
~~~~~~~~~~~

The easiest way to install Symfony CMF is is using `Composer`_. Get it using

.. code-block:: bash

    $ curl -sS https://getcomposer.org/installer | php
    $ sudo mv composer.phar /usr/local/bin/composer

and then get the Symfony CMF code with it (this may take a while):

.. code-block:: bash

    $ composer create-project symfony-cmf/standard-edition <path-to-install> "~1.2"
    $ cd <path-to-install>

.. note::

    The path ``<path-to-install>`` should either be inside your web server doc
    root or you need to configure a virtual host for ``<path-to-install>``.

This will clone the Standard Edition and install all the dependencies and run
some initial commands. These commands require write permissions to the
``var/cache`` and ``var/logs`` directory. In case the final commands end up
giving permissions errors, please follow the `guidelines in the Symfony Book`_
to configure the permissions and then run the ``install`` command:

.. code-block:: bash

    $ composer install

2) GIT
~~~~~~

You can also install the Standard Edition using GIT. Just clone the repository
from github:

.. code-block:: bash

    $ git clone git://github.com/symfony-cmf/standard-edition.git <path-to-install>
    $ cd <path-to-install>

You still need Composer to get the dependencies. To get the correct
dependencies, use the ``install`` command:

.. code-block:: bash

    $ composer install

To try out things, you can accept the default values for all questions you are
asked about the parameters.yml. Revisit that file later when you know more
about Jackalope.

Setup
-----

You are almost there. A few more steps need to be done to be ready.

Set up the Database
~~~~~~~~~~~~~~~~~~~

The next step is to set up the database. If you want to use SQLite as your
database backend just go ahead and run the following:

.. code-block:: bash

    $ php bin/console doctrine:database:create
    $ php bin/console doctrine:phpcr:init:dbal --force
    $ php bin/console doctrine:phpcr:repository:init
    $ php bin/console doctrine:phpcr:fixtures:load

The first command will create a file called ``app.sqlite`` inside your app
folder, containing the database content. The two commands after it will setup
PHPCR and the final command will load some fixtures, so you can access the
Standard Edition using a web server.

Preparing Assetic
~~~~~~~~~~~~~~~~~

To use the frontend editing in ``prod`` environment, you need to tell Assetic
to dump the assets to the filesystem:

.. code-block:: bash

    $ php bin/console --env=prod assetic:dump

Configure a Webserver
~~~~~~~~~~~~~~~~~~~~~

The project is now ready to be served by your web server. If you have PHP 5.4
installed you can alternatively use the PHP internal web server:

.. code-block:: bash

    $ php bin/console server:run

And then access the CMF via:

.. code-block:: text

    http://localhost:8000

If you run an Apache installation as described in the `Symfony cookbook article on setup`_,
your URL will look like this:

.. code-block:: text

    http://localhost/app_dev.php

.. note::

    Adding the ``app_dev.php`` to the url in your browser is important to actually
    see the test page. Because the AcmeDemoBundle is only configured to work with the
    Development Environment. (If you have a look at ``AppKernel.php`` you can easily
    spot why)

.. sidebar:: Using Other Database Backends

    If you prefer to use another database backend, for example MySQL, run the
    configurator (point your browser to ``http://localhost:8000/config.php``)
    or set your database connection parameters in ``app/config/parameters.yml``.
    Make sure you leave the ``database_path`` property at ``null`` in order to
    use another driver than SQLite. Leaving the field blank in the
    web-configurator will set it to ``null``. You also need to uncomment lines
    in ``app/config/config.yml`` in section ``doctrine.dbal``.

.. note::

    The proper term to use for the default database of the CMF is
    *content repository*. The idea behind this name is essentially to describe a
    specialized database created specifically for content management systems.
    The acronym *PHPCR* actually stands for *PHP content repository*. But as
    mentioned before, the CMF is storage agnostic so its possible to combine
    the CMF with other storage mechanism, like Doctrine ORM, Propel etc.

Overview
--------

This section will help you understand the basic parts of Symfony CMF Standard
Edition (SE) and how they work together to provide the default pages you can
see when browsing the Symfony CMF SE installation.

It assumes you have already installed Symfony CMF SE and have carefully read
`the Symfony2 book`_.

AcmeMainBundle and SimpleCmsBundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony CMF SE comes with a default AcmeDemoBundle to help you get started,
similar to the AcmeDemoBundle provided by Symfony2 SE. This gives you some demo
pages viewable in your browser.

.. sidebar:: Where are the Controllers?

    AcmeDemoBundle doesn't include controllers or configuration files as you
    might expect. It contains little more than a Twig file and `Fixtures`_
    data that was loaded into your database during installation. The biggest
    chunk of code is the ``UnitBlock`` that provides a document for an example
    block.

    The controller logic is actually provided by the relevant CMF bundles,
    as described below.

There are several bundles working together in order to turn the fixture data
into a browsable website. The overall, simplified process is:

* When a request is received, the :doc:`Symfony CMF Routing's Dynamic Router <routing>`
  is used to handle the incoming request;
* The Dynamic Router is able to match the requested URL to a ``Page`` document
  provided by SimpleCmsBundle and stored inside the database;
* The retrieved document information is used to determine which controller to
  pass it on to, and which template to use;
* As configured, the retrieved document is passed to ``ContentController``
  provided by the ContentBundle, which render document into ``layout.html.twig``
  of the AcmeMainBundle.

Again, this is simplified view of a very simple CMS built on top of Symfony
CMF. To fully understand all the possibilities of the CMF, continue reading
this Book section.

If you want to review the contents of the PHPCR database you can use the
following commands:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:node:dump
    $ php bin/console doctrine:phpcr:node:dump --props
    $ php bin/console doctrine:phpcr:node:dump /path/to/node

The above examples respectively show a summary, a detailed view, and a summary
of a node and all its children (instead of starting at the root node).

Don't forget to look at the ``--help`` output for more possibilities:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:node:dump --help

Adding new pages
~~~~~~~~~~~~~~~~

Symfony CMF SE does not provide any admin tools to create new pages. If you
are interested in adding an admin UI one solution can be found in
:doc:`../tutorial/sonata-admin`. However, if all you want
is a simple way to add new pages that you can then edit via the in-line
editing, then you can use the SimpleCmsBundle ``page`` migrator. For example,
to add a page called "Testing", creating a file called
``app/Resources/data/pages/test.yml`` with the following contents:

.. code-block:: yaml

    label: "Testing"
    title: "Testing"
    body: |
        <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit.</p>
        <p>Morbi eleifend, ipsum eget facilisis lacinia, lorem dui venenatis quam, at vulputate purus erat sit amet elit.</p>

The contents of this file can be loaded into the PHPCR database by calling:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:migrator:migrate page --identifier=/cms/simple/test

Note that the above identifier is mapped to
``app/Resources/data/pages/test.yml`` by stripping off the ``basepath``
configuration of the SimpleCmsBundle (which defaults to ``/cms/simple``).

Therefore if you want to define a child page ``foo`` for ``/cms/simple/test``
you would need to create a file ``app/Resources/data/pages/test/foo.yml``
and then run the following command:

.. code-block:: bash

    $ php bin/console doctrine:phpcr:migrator:migrate page --identifier=/cms/simple/test/foo

.. _`cmf.liip.ch`: http://cmf.liip.ch
.. _`Requirements for running Symfony2`: https://symfony.com/doc/current/reference/requirements.html
.. _`SQLite`: http://www.sqlite.org/
.. _`Composer`: https://getcomposer.org/
.. _`guidelines in the symfony book`: https://symfony.com/doc/current/book/installation.html#book-installation-permissions
.. _`the Symfony2 book`: https://symfony.com/doc/current/book/index.html
.. _`Fixtures`: https://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
.. _`Symfony cookbook article on setup`: https://symfony.com/doc/current/cookbook/configuration/web_server_configuration.html
