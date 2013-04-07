Blog Tutorial
=============

This tutorial will show you how to create a simple blog application in the Symfony CMF
using the `BlogBundle`.

Getting started
---------------

First of all we need to create a new project. The easiest way to do this is to use composer
as follows:

.. code-block:: bash

    $ php composer.phar create-project --stability dev symfony-cmf/standard-edition /path/to/project

Replace ``path/to/project`` with the directory where you want your blog application to live.

Now lets add the blog bundle to `composer.json`

.. code-block:: javascript


    {
        "name": "symfony-cmf/standard-edition",
        [...]
        "require": {
            [...],
            "symfony-cmf/blog-bundle": "dev-auto_route"
        },
        [...]
    }

.. note:: 
    
    Remember that JSON requires the last item in a list to NOT have a trailing comma!

Next add the blog bundle to ``AppKernel.php``::

    <?php

    // ./app/AppKernel.php

    class AppKernel
    {
        // ...
        $bundles = array(
            // ...
            new Symfony\Cmf\Bundle\BlogBundle\SymfonyCmfBlogBundle()
    }

Now, if you try to invoke the console you should receieve an error:

.. code-block:: bash

    $ ./app/console
    
      [Symfony\Component\Config\Definition\Exception\InvalidConfigurationException]  
      The child node "blog_basepath" at path "symfony_cmf_blog" must be configured.  
                                                                                 
Great. So accordingly we now have to tell the blog bundle where in the PHPCR-PDM document
tree it should store the blog. Add the following to `config.yml`:

.. code-block:: yaml

    // ./app/config/config.yml
    symfony_cmf_blog:
        blog_basepath: /cms/ << where ?
