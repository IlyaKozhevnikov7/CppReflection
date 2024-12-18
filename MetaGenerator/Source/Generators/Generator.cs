using System.Text;

namespace MetaGenerator
{
    public abstract class Generator<TContext>
    {
        protected TContext Context { get; private set; }

        public void SetContext(TContext context)
        {
            Context = context;
        }

        public abstract void Generate(StringBuilder builder);
    }

    public class Generator
    {
        public static void Launch<T, TContext>(StringBuilder builder, TContext context) where T : Generator<TContext>, new()
        {
            var generator = new T();
            generator.SetContext(context);
            generator.Generate(builder);
        }
    }
}
