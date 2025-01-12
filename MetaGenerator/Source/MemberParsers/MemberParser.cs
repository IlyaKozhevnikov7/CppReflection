using PCRE;
using System;
using System.Collections.Generic;

namespace MetaGenerator
{
    public abstract class MemberParser
    {
        public abstract string Text { get; }

        protected static IEnumerable<PcreMatch> FindMatches(string pattern, string text) => new PcreRegex(pattern).Matches(text);

        protected static AttributeInfo[] ParseAttributes(string attributeDefinitions)
        {
            if (attributeDefinitions.Length == 2)
                return null;

            attributeDefinitions = attributeDefinitions[1..^1];

            int bracketNesting = 0;
            int quoteNesting = 0;
            
            (int, int) attributeNameBound = (0, attributeDefinitions.Length);
            (int, int) attributeArgsBound = (0, 0);

            List<AttributeInfo> attributeInfos = new();

            for (int i = 0; i < attributeDefinitions.Length; ++i)
            {
                char code = attributeDefinitions[i];

                switch (code)
                {
                    case '(':
                        if (bracketNesting == 0)
                            attributeNameBound.Item2 = i;

                        ++bracketNesting;

                        if (bracketNesting == 1)
                            attributeArgsBound.Item1 = i + 1;

                        break;

                    case ')':
                        --bracketNesting;

                        if (bracketNesting == 0)
                            attributeArgsBound.Item2 = i;

                        break;

                    case '<':
                        ++quoteNesting;
                        break;

                    case '>':
                        --quoteNesting;
                        break;

                    case ',':
                        {
                            if (bracketNesting == 0 && quoteNesting == 0)
                            {
                                attributeNameBound.Item2 = i;

                                attributeInfos.Add(new AttributeInfo
                                {
                                    name = attributeDefinitions.Substring(attributeNameBound.Item1, attributeNameBound.Item2 - attributeNameBound.Item1),
                                    args = attributeDefinitions.Substring(attributeArgsBound.Item1, attributeArgsBound.Item2 - attributeArgsBound.Item1)
                                });

                                attributeNameBound.Item1 = i + 1;
                            }
                        }
                        break;
                }
            }

            if (attributeNameBound.Item2 < attributeNameBound.Item1)
                attributeNameBound.Item2 = attributeDefinitions.Length;

            attributeInfos.Add(new AttributeInfo
            {
                name = attributeDefinitions.Substring(attributeNameBound.Item1, attributeNameBound.Item2 - attributeNameBound.Item1),
                args = attributeDefinitions.Substring(attributeArgsBound.Item1, attributeArgsBound.Item2 - attributeArgsBound.Item1)
            });

            return attributeInfos.Count > 0 ? attributeInfos.ToArray() : null;
        }
    }


    public abstract class PrimaryMemberParser : MemberParser
    {
        public HeaderParser HeaderParser { get; set; }
        public override string Text => HeaderParser.Text;

        public PrimaryMemberParser(HeaderParser headerParser)
        {
            HeaderParser = headerParser;
        }
    }

    public abstract class SecondaryMemberParser : MemberParser
    {
        private string _text;

        public override string Text => _text;

        public SecondaryMemberParser(string text)
        {
            _text = text;
        }
    }
}
