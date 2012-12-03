Installing the Symfony CMF
==========================

The goal of this tutorial is to install all the CMF components with the minimum
necessary configuration. From there, you can begin incorporating CMF functionality
into your application as needed. Once completed, you should have a minimal 
functional development setup, with some demonstration pages.

If this is your first encounter with the Symfony CMF it would be a good idea to first take a
look at:

- `The Big Picture <http://slides.liip.ch/static/2012-01-17_symfony_cmf_big_picture.html#1>`_
- The `CMF sandbox environment <https://github.com/symfony-cmf/cmf-sandbox>`_, a pre-installed Symfony / CMF application containing all CMF components.
- The online sandbox demo at `cmf.liip.ch <http://cmf.liip.ch>`_

.. note::

    For other Symfony CMF installation guides, please read:
    - The cookbook entry on :doc:`/cookbook/installing-cmf-sandbox` for instructions on how to install a more complete demo instacnce of Symfony CMF.
    - :doc:`/tutorials/installing-cmf-core` for step-by-step installation and configuration details (if you want to know all the details)

.. index:: RoutingExtraBundle, CoreBundle, MenuBundle, ContentBundle, SonataBlockBundle, KnpMenuBundle

Preconditions
-------------

As Symfony CMF is based on Symfony 2, you should make sure you
meet the `Requirements for running Symfony2 <http://symfony.com/doc/current/reference/requirements.html>`_.
Aditionally, you need to have `Sqlite <http://www.sqlite.org/>`_ 
installed, since it's used as the default storage medium. 

.. note::

    By default, Symfony CMF uses Jackalope + Doctrine DBAL, and Sqlite as
    the underlying DB. However, Symfony CMF is storage agnostic, which means
    you can use one of several available data storage mechanisms without
    having to rewrite your code. For more information on the different
    available mechanisms and how to install and configure them, refer to
    :doc:`/tutorials/installing-configuring-doctrine-phpcr-odm`

`Git <http://git-scm.com/>`_ and `Curl <http://curl.haxx.se/>`_ are also needed to follow the installation steps listed below.


Installation
------------

The easiest way to install Symfony CMF is is using `Composer <http://getcomposer.org/>`_.
Get it using

.. code-block:: bash

    curl -s http://getcomposer.org/installer | php --

and then get the Symfony CMF code with it (this may take a while)

.. code-block:: bash

    php composer.phar create-project symfony-cmf/standard-edition <path-to-install>
    cd <path-to-install>
    
Install all the required bundles (this may take a while)

.. code-block:: bash

    php composer.phar install

Once you have downloaded the necessary resources, it's time to setup the database:

.. code-block:: bash

    app/console doctrine:database:create
    app/console doctrine:phpcr:init:dbal
    app/console doctrine:phpcr:register-system-node-types
    app/console doctrine:phpcr:fixtures:load

This will create a file called app.sqlite inside your app folder, containing
the database content needed by the sandbox.

The sandbox should now be accessible on your web server.

.. code-block:: text

    http://localhost/
    