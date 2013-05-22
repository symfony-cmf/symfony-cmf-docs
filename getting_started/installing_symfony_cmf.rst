.. index::
    single: Standard Edition, install

Installing the Symfony CMF Standard Edition
===========================================

The Symfony CMF Standard Edition (SE) is a distribution based on all the
main components needed for most common use cases.

The goal of this tutorial is to install the CMF components, with the
minimum necessary configuration and some very simple examples, into a working
Symfony2 application.

We will then walk you through the components you have installed.
This can be used to familiarize yourself with the CMF or
as a starting point for a new custom application.

If this is your first encounter with the Symfony CMF it would be a good idea
to first take a look at:

* `The Big Picture`_
* The online sandbox demo at `cmf.liip.ch`_

.. note::

    For other Symfony CMF installation guides, please read:

    * The cookbook entry on :doc:`../cookbook/installing-cmf-sandbox` for
      instructions on how to install a more complete demo instance of Symfony
      CMF.
    * :doc:`../tutorials/installing-cmf-core` for step-by-step installation
      and configuration details of just the core components into an existing
      Symfony application.

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
    :doc:`../tutorials/installing-configuring-doctrine-phpcr-odm`

`Git`_ and `Curl`_ are also needed to follow the installation steps listed
below.

Installation
------------

The easiest way to install Symfony CMF is is using `Composer`_. Get it using

.. code-block:: bash

    $ curl -sS https://getcomposer.org/installer | php
    $ sudo mv composer.phar /usr/local/bin/composer

and then get the Symfony CMF code with it (this may take a while)

.. code-block:: bash

    $ php composer.phar create-project symfony-cmf/standard-edition <path-to-install> --stability=dev
    $ cd <path-to-install>

.. note::

    The path ``<path-to-install>`` should either inside your web server doc
    root or configure a virtual host for ``<path-to-install>``.

This will clone the standard edition and install all the dependencies and run
some initial commands.  These commands require write permissions to the
``app/cache`` and ``app/logs`` directory. In case the final commands end up
giving permissions errors, please follow the `guidelines in the symfony book`_
for configuring the permissions and then run the ``composer.phar install``
command mentioned below.

If you prefer you can also just clone the project:

.. code-block:: bash

    $ git clone git://github.com/symfony-cmf/symfony-cmf-standard.git <dir-name>
    $ cd <dir-name>

If there were problems during the ``create-project`` command, if you used
``git clone`` or if you updated the checkout later, always run the following
command to update the dependencies:

.. code-block:: bash

    $ php composer.phar install

The next step is to set up the database. If you want to use SQLite as your
database backend just go ahead and run the following:

.. code-block:: bash

    $ php app/console doctrine:database:create
    $ php app/console doctrine:phpcr:init:dbal
    $ php app/console doctrine:phpcr:repository:init
    $ php app/console doctrine:phpcr:fixtures:load

This will create a file called ``app.sqlite`` inside your app folder,
containing the database content.

The project should now be accessible on your web server. If you have PHP 5.4
installed you can alternatively use the PHP internal web server:

.. code-block:: bash

    $ php app/console server:run

And then access the CMF via:

.. code-block:: text

    http://localhost:8000

If you prefer to use another database backend, for example MySQL, run the
configurator (point your browser to ``/web/config.php``) or set your database
connection parameters in ``app/config/parameters.yml``. Make sure you leave
the ``database_path`` property at ``null`` in order to use another driver than
SQLite. Leaving the field blank in the web-configurator should set it to
``null``.

Overview
--------

This section will help you understand the basic parts of Symfony CMF Standard
Edition (SE) and how they work together to provide the default pages you can
see when browsing the Symfony CMF SE installation.

It assumes you have already installed Symfony CMF SE and have carefully read
`the Symfony2 book`_.

AcmeMainBundle and SimpleCMSBundle
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Symfony CMF SE comes with a default AcmeMainBundle to help you get started,
similar to the AcmeDemoBundle provided by Symfony2. This gives you some demo
pages viewable in your browser.

.. sidebar:: Where are the Controllers?

    AcmeMainBundle doesn't include controllers or configuration files as you
    might expect. It contains little more than a Twig file and `Fixtures`_
    data that was loaded into your database during installation.

    The controller logic is actually provided by the relevant CMF bundles,
    as described below.

There are several bundles working together in order to turn the fixture data
into a browsable website. The overall, simplified process is:

* When a request is received, the :doc:`Symfony CMF Routing's Dynamic Router <routing>`
  is used to handle the incoming request;
* The Dynamic Router is able to match the requested URL with a specific
  ContentBundle's Content stored in the database;
* The retrieved content's information is used to determine which controller to
  pass it on to, and which template to use;
* As configured, the retrieved content is passed to ContentBundle's
  ``ContentController``, which will handle it and render AcmeMainBundle's
  ``layout.html.twig``.

Again, this is simplified view of a very simple CMS built on top of Symfony
CMF.  To fully understand all the possibilities of the CMF, a careful look
into each component is needed.

If you want to review the contents of the PHPCR database you can use the
following commands:

.. code-block:: bash

    $ php app/console doctrine:phpcr:dump
    $ php app/console doctrine:phpcr:dump --props
    $ php app/console doctrine:phpcr:dump /path/to/node

The above examples respectively show a summary, a detailed view, and a summary
of a node and all its children (instead of starting at the root node).

Don't forget to look at the ``--help`` output for more possibilities:

.. code-block:: bash

    $ php app/console doctrine:phpcr:dump

Adding new pages
~~~~~~~~~~~~~~~~

Symfony CMF SE does not provide any admin tools to create new pages. If you
are interested in adding an admin UI have a look at
:doc:`../tutorials/creating-cms-using-cmf-and-sonata`. However if all you want
is a simple way to add new pages that you can then edit via the inline
editing, then you can use the SimpleCmsBundle ``page`` migrator. The Symfony
CMF SE ships with an example YAML file stored in
``app/Resources/data/pages/test.yml``. The contents of this file can be loaded
into the PHPCR database by calling:

.. code-block:: bash

    $ php app/console doctrine:phpcr:migrator page --identifier=/cms/simple/test

Note that the above identifier is mapped to
``app/Resources/data/pages/test.yml`` by stripping off the ``basepath``
configuration of the SimpleCmsBundle, which defaults to ``/cms/simple``.
Therefore if you want to define a child page ``foo`` for ``/cms/simple/test``
you would need to create a file ``app/Resources/data/pages/test/foo.yml``
and then run the following command:

.. code-block:: bash

    $ php app/console doctrine:phpcr:migrator page --identifier=/cms/simple/test/foo

.. _`The Big Picture`: http://slides.liip.ch/static/2012-01-17_symfony_cmf_big_picture.html#1
.. _`cmf.liip.ch`: http://cmf.liip.ch
.. _`Requirements for running Symfony2`: http://symfony.com/doc/current/reference/requirements.html
.. _`SQLite`: http://www.sqlite.org/
.. _`Git`: http://git-scm.com/
.. _`Curl`: http://curl.haxx.se/
.. _`Composer`: http://getcomposer.org/
.. _`guidelines in the symfony book`: http://symfony.com/doc/master/book/installation.html#configuration-and-setup
.. _`the Symfony2 book`: http://symfony.com/doc/current/book/
.. _`Fixtures`: http://symfony.com/doc/current/bundles/DoctrineFixturesBundle/index.html
