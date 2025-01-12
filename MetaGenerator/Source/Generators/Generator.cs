using System.Text;

namespace MetaGenerator
{
    public abstract class Generator
    {
        protected const string GENERATED_FILE_HEADER_TEXT = @"/*============================================================
        This file was created using MetaGenerator
    !!!!! DO NOT MAKE ANY CHANGES TO THIS FILE !!!!!
============================================================*/
";

        public abstract void Run();

        public static void Launch<T>() where T : PrimaryGenerator, new()
        {
            var generator = new T();
            generator.Run();
        }

        public static void Launch<T, TContext>(TContext context) where T : PrimaryGenerator<TContext>, new()
        {
            var generator = new T();
            generator.SetContext(context);
            generator.Run();
        }

        public static void Launch<T, TContext>(StringBuilder builder, TContext context) where T : SecondaryGenerator<TContext>, new()
        {
            var generator = new T();
            generator.SetContext(context);
            generator.SetBuilder(builder);
            generator.Run();
        }
    }

    public abstract class Generator<TContext> : Generator
    {
        public TContext Context { get; private set; }

        public void SetContext(TContext context) => Context = context;
    }

    public abstract class PrimaryGenerator : Generator
    {
        protected StringBuilder Builder { get; private set; } = new();
    }

    public abstract class PrimaryGenerator<TContext> : Generator<TContext>
    {
        protected StringBuilder Builder { get; private set; } = new();
    }

    public abstract class SecondaryGenerator<TContext> : Generator<TContext>
    {
        protected StringBuilder Builder { get; private set; }

        public void SetBuilder(StringBuilder builder) => Builder = builder;
    }
}
